/*
cd ~/git/apgar/gstream
gcc testvideo.c -o testvideo `pkg-config --cflags --libs gstreamer-1.0`;./testvideo;
     OR
gcc -g testvideo.c -o testvideo `pkg-config --cflags --libs gstreamer-1.0`; gdb ./testvideo;

launch command
gst-launch-1.0 videotestsrc ! autovideosink

useful links
*/

#include <gst/gst.h>

typedef struct _App {
    GstElement *pipeline;
    GstBus *bus;
    GstElement *testvideosrc;
    GstElement *autovideosink;
} App;

static void cleanup (App *data);

int main(int argc, char *argv[]) {
    App app;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    gst_init (&argc, &argv);

    app.pipeline = gst_pipeline_new ("room");
    app.testvideosrc = gst_element_factory_make ("videotestsrc", "videotestsrc");
    app.autovideosink = gst_element_factory_make ("autovideosink", "autovideosink");

    if (!app.pipeline || !app.testvideosrc || !app.autovideosink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    if(!gst_bin_add(GST_BIN(app.pipeline), app.testvideosrc)){
        g_printerr ("could not add testvideosrc to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.autovideosink)){
        g_printerr ("could not add autovideosink to pipeline.\n");
        return -1;
    }

    if(!gst_element_link(app.testvideosrc, app.autovideosink)){
        g_printerr ("could not link testvideosrc to autovideosink.\n");
        return -1;
    }

    ret = gst_element_set_state (app.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        cleanup(&app);
        return -1;
    }

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
        gst_object_unref (app->bus);
    }
    if(app->pipeline != NULL){
        gst_element_set_state (app->pipeline, GST_STATE_NULL);
        gst_object_unref (app->pipeline);
    }
}
