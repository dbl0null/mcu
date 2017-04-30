/*
cd ~/git/apgar/gstream
gcc staticimage.c -o staticimage `pkg-config --cflags --libs gstreamer-1.0`;./staticimage;
     OR
gcc -g staticimage.c -o staticimage `pkg-config --cflags --libs gstreamer-1.0`; gdb ./staticimage;

launch command
gst-launch-1.0 -v filesrc location=img/bg-1280x720.png ! decodebin ! imagefreeze ! autovideosink

useful links
https://gstreamer.freedesktop.org/documentation/application-development/highlevel/playback-components.html
https://gstreamer.freedesktop.org/documentation/tutorials/basic/dynamic-pipelines.html
*/

#include <gst/gst.h>

typedef struct _App {
    GstElement *pipeline;
    GstBus *bus;
    GstElement *imagesource;
    GstElement *decodebin;
    GstElement *imagefreeze;
    GstElement *autovideosink;
} App;

static void decodebin_pad_added_handler (GstElement *decodebin, GstPad *pad, App *data);
static void cleanup (App *data);

int main(int argc, char *argv[]) {
    App app;
    GstMessage *msg;
    GstStateChangeReturn ret;
    gboolean terminate = FALSE;

    gst_init (&argc, &argv);

    app.pipeline = gst_pipeline_new ("room");
    app.imagesource = gst_element_factory_make ("filesrc", "imagesource");
    app.decodebin = gst_element_factory_make ("decodebin", "decoder");
    app.imagefreeze  = gst_element_factory_make ("imagefreeze", "freezer");
    app.autovideosink = gst_element_factory_make ("autovideosink", "videosink");

    if (!app.pipeline || !app.imagesource || !app.decodebin || !app.imagefreeze || !app.autovideosink) {
        g_printerr ("Not all elements could be created.\n");
        return -1;
    }

    if(!gst_bin_add(GST_BIN(app.pipeline), app.imagesource)){
        g_printerr ("could not add imagesource to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.decodebin)){
        g_printerr ("could not add decodebin to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.imagefreeze)){
        g_printerr ("could not add imagefreeze to pipeline.\n");
        return -1;
    }
    if(!gst_bin_add(GST_BIN(app.pipeline), app.autovideosink)){
        g_printerr ("could not add autovideosink to pipeline.\n");
        return -1;
    }

    if(!gst_element_link(app.imagesource, app.decodebin)){
        g_printerr ("could not link imagesource to decodebin.\n");
        return -1;
    }

    g_object_set (G_OBJECT(app.imagesource), "location", "img/bg-1280x720.png", NULL);
    g_signal_connect (app.decodebin, "pad-added", G_CALLBACK (decodebin_pad_added_handler), &app);

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

static void decodebin_pad_added_handler (GstElement *decodebin, GstPad *new_pad, App *app) {

    GstPad *imagefreeze_pad = gst_element_get_static_pad (app->imagefreeze, "sink");
    GstCaps *new_pad_caps = NULL;
    GstStructure *new_pad_struct = NULL;
    const gchar *new_pad_type = NULL;

    /* If our converter is already linked, we have nothing to do here */
    g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (decodebin));
    if (gst_pad_is_linked (imagefreeze_pad)) {
        g_print ("We are already linked. Ignoring.\n");
        goto exit;
    }

    /* Check the new pad's type */
    new_pad_caps = gst_pad_query_caps (new_pad, NULL);
    new_pad_struct = gst_caps_get_structure (new_pad_caps, 0);
    new_pad_type = gst_structure_get_name (new_pad_struct);
    if (!g_str_has_prefix (new_pad_type, "video/x-raw")) {
        g_printerr ("It has type '%s' which is not raw video. Ignoring.\n", new_pad_type);
        goto exit;
    }

    /* Attempt the links */
    GstPadLinkReturn ret = gst_pad_link(new_pad, imagefreeze_pad);
    if (GST_PAD_LINK_FAILED (ret)) {
        g_printerr ("Type is '%s' but link failed.\n", new_pad_type);
    } else {
        g_print ("Link succeeded (type '%s').\n", new_pad_type);
    }

    if(!gst_element_link(app->imagefreeze, app->autovideosink)){
        g_printerr ("could not link imagefreeze to autovideosink.\n");
        goto exit;
    }

    exit:
    if (new_pad_caps != NULL){
        gst_caps_unref (new_pad_caps);
    }
    gst_object_unref (imagefreeze_pad);
}
