/*
cd ~/git/apgar/gstream
gcc staticimagechange.c -o staticimagechange `pkg-config --cflags --libs gstreamer-1.0`;./staticimagechange;
     OR
gcc -g staticimagechange.c -o staticimagechange `pkg-config --cflags --libs gstreamer-1.0`; gdb ./staticimagechange;

launch command
gst-launch-1.0 multifilesrc location=img/bg-1280x720.jpg caps=image/jpeg,framerate=30/1 ! jpegdec ! videoconvert ! autovideosink
gst-launch-1.0 multifilesrc location=img/bg-1280x720.jpg caps=image/jpeg,framerate=30/1 ! jpegdec ! videoconvert ! videoscale ! video/x-raw,width=1280,height=720 ! autovideosink

gst-launch-1.0 multifilesrc location=img/bg-1280x720.jpg caps=image/jpeg,framerate=30/1 ! jpegdec ! videoconvert ! autovideosink

useful links
https://gstreamer.freedesktop.org/documentation/application-development/highlevel/playback-components.html
https://gstreamer.freedesktop.org/documentation/tutorials/basic/dynamic-pipelines.html
*/

#include <gst/gst.h>

typedef struct _App {
    GstElement *pipeline;
    GstBus *bus;
    GstElement *imagesource;
    GstElement *jpegdec;
    GstElement *videoconvert;
    GstElement *autovideosink;
} App;

static void cleanup (App *data);

int main(int argc, char *argv[]) {
    App app;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    g_print("init...");
    gst_init (&argc, &argv);

    app.pipeline = gst_pipeline_new ("room");
    app.imagesource = gst_element_factory_make ("multifilesrc", "imagesource");
    app.jpegdec = gst_element_factory_make ("jpegdec", "decoder");
    app.videoconvert  = gst_element_factory_make ("videoconvert", "videoconvert");
    app.autovideosink = gst_element_factory_make ("autovideosink", "videosink");

    if (!app.pipeline || !app.imagesource || !app.jpegdec || !app.videoconvert || !app.autovideosink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    g_print("add to pipleline...");
    if(!gst_bin_add(GST_BIN(app.pipeline), app.imagesource)){
        g_printerr ("could not add imagesource to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.jpegdec)){
        g_printerr ("could not add jpegdec to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.videoconvert)){
        g_printerr ("could not add videoconvert to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.autovideosink)){
        g_printerr ("could not add autovideosink to pipeline.\n");
        return -1;
    }

    g_print("linking...");
    if(!gst_element_link(app.imagesource, app.jpegdec)){
        g_printerr ("could not link imagesource to jpegdec.\n");
        return -1;
    }

    if(!gst_element_link(app.jpegdec, app.videoconvert)){
        g_printerr ("could not link jpegdec to videoconvert.\n");
        return -1;
    }

    if(!gst_element_link(app.videoconvert, app.autovideosink)){
        g_printerr ("could not link videoconvert to autovideosink.\n");
        return -1;
    }

    g_object_set (G_OBJECT(app.imagesource), "location", "img/bg-1280x720.jpg", NULL);
    // g_object_set (G_OBJECT(app.imagesource), "caps", "image/jpeg,framerate=30/1", NULL);

    g_print("playing...");
    ret = gst_element_set_state (app.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        cleanup(&app);
        return -1;
    }

    // change video image here
    g_object_set (G_OBJECT(app.imagesource), "location", "img/bg2-1280x720.jpg", NULL);

    g_print("bus...");
    app.bus = gst_element_get_bus (app.pipeline);
    do {
        msg = gst_bus_timed_pop_filtered (app.bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_STATE_CHANGED | GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
        if (msg != NULL) {
            GError *err;
            gchar *debug_info;

            switch (GST_MESSAGE_TYPE (msg)) {
                case GST_MESSAGE_ERROR:
                    gst_message_parse_error (msg, &err, &debug_info);
                    g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
                    g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
                    g_clear_error (&err);
                    g_free (debug_info);
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_EOS:
                    g_print ("End-Of-Stream reached.\n");
                    terminate = TRUE;
                    break;
                case GST_MESSAGE_STATE_CHANGED:
                    /* We are only interested in state-changed messages from the pipeline */
                    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (app.pipeline)) {
                        GstState old_state, new_state, pending_state;
                        gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
                        g_print ("Pipeline state changed from %s to %s:\n",
                        gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
                    }
                    break;
                default:
                    /* We should not reach here */
                    g_printerr ("Unexpected message received.\n");
                    break;
            }
            gst_message_unref (msg);
        }
    } while (!terminate);

    cleanup(&app);
    return 0;
}

static void cleanup (App *app) {
    if(app->bus != NULL){
        g_printerr ("Cleaning bus.\n");
        gst_object_unref (app->bus);
    }
    if(app->pipeline != NULL){
        g_printerr ("Cleaning pipeline.\n");
        gst_element_set_state (app->pipeline, GST_STATE_NULL);
        gst_object_unref (app->pipeline);
    }
}
