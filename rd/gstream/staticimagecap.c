/*
cd ~/git/apgar/gstream
gcc staticimagecap.c -o staticimagecap `pkg-config --cflags --libs gstreamer-1.0`;./staticimagecap;
     OR
gcc -g staticimagecap.c -o staticimagecap `pkg-config --cflags --libs gstreamer-1.0`; gdb ./staticimagecap;

launch command
gst-launch-1.0 -v multifilesrc location=img/bg-1280x720.jpg caps=image/jpeg,framerate=30/1 ! jpegdec ! videoconvert ! videoscale ! video/x-raw,width=640,height=480 ! autovideosink
gst-launch-1.0 -v multifilesrc location=img/bg-1280x720.jpg caps=image/jpeg,framerate=30/1 ! jpegdec ! videoconvert ! videoscale ! video/x-raw,width=640,height=480 ! videorate ! video/x-raw,framerate=15/1 ! autovideosink

useful links
https://gstreamer.freedesktop.org/documentation/application-development/highlevel/playback-components.html
https://gstreamer.freedesktop.org/documentation/tutorials/basic/dynamic-pipelines.html
*/

#include <gst/gst.h>

typedef struct _App {
    GstElement *pipeline;
    GstBus *bus;
    GstElement *multifilesrc;
    GstElement *jpegdec;
    GstElement *videoconvert;
    GstElement *videoscale;
    GstElement *videorate;
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
    app.multifilesrc = gst_element_factory_make ("multifilesrc", "multifilesrc");
    app.jpegdec = gst_element_factory_make ("jpegdec", "decoder");
    app.videoconvert  = gst_element_factory_make ("videoconvert", "videoconvert");
    app.videoscale  = gst_element_factory_make ("videoscale", "videoscale");
    app.videorate  = gst_element_factory_make ("videorate", "videorate");
    app.autovideosink = gst_element_factory_make ("autovideosink", "videosink");

    if (!app.pipeline || !app.multifilesrc || !app.jpegdec || !app.videoconvert || !app.videoscale || ! app.videorate || !app.autovideosink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    g_print("add to pipleline...");
    gst_bin_add_many(GST_BIN(app.pipeline),
        app.multifilesrc,
        app.jpegdec,
        app.videoconvert,
        app.videoscale,
        app.videorate,
        app.autovideosink,
        NULL);

    g_print("linking...");

    g_object_set (G_OBJECT(app.multifilesrc), "location", "img/bg-1280x720.jpg", NULL);
    GstCaps *caps = gst_caps_new_simple ("image/jpeg",
        "width", G_TYPE_INT, 1280,
        "height", G_TYPE_INT, 720,
        "framerate", GST_TYPE_FRACTION, 30, 1,
        NULL);
    if(!gst_element_link_filtered (app.multifilesrc, app.jpegdec, caps)){
        g_printerr ("could not link multifilesrc to jpegdec caps.\n");
        return -1;
    }
    gst_caps_unref (caps);

    if(!gst_element_link(app.jpegdec, app.videoconvert)){
        g_printerr ("could not link jpegdec to videoconvert.\n");
        return -1;
    }

    if(!gst_element_link(app.videoconvert, app.videoscale)){
        g_printerr ("could not link videoconvert to videoscale.\n");
        return -1;
    }

    GstCaps *caps2 = gst_caps_new_simple ("video/x-raw",
        "width", G_TYPE_INT, 640,
        "height", G_TYPE_INT, 480,
        NULL);
    if(!gst_element_link_filtered (app.videoscale, app.videorate, caps2)){
        g_printerr ("could not link videoscale to videorate caps.\n");
        return -1;
    }
    gst_caps_unref (caps2);

    GstCaps *caps3 = gst_caps_new_simple ("video/x-raw",
        "framerate", GST_TYPE_FRACTION, 15, 1,
        NULL);
    if(!gst_element_link_filtered (app.videorate, app.autovideosink, caps3)){
        g_printerr ("could not link videorate to autovideosink caps.\n");
        return -1;
    }
    gst_caps_unref (caps3);

    g_print("playing...");
    ret = gst_element_set_state (app.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        cleanup(&app);
        return -1;
    }

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
