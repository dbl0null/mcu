#ifndef STREAMDATA_H
#define STREAMDATA_H

#include <gst/gst.h>

namespace gstream {

    struct RobotStreamData {
        std::string ConferenceId;
        std::string DefaultView;
        GstElement *Pipeline;
        GstBus *Bus;
        GstElement *ImageSource;
        GstElement *JpegDec;
        GstElement *VideoConvert;
        GstElement *AutoVideoSink;
    };
}

#endif
