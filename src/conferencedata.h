#ifndef CONFERENCEDATA_H
#define CONFERENCEDATA_H

#include <gst/gst.h>

namespace mcu {

    struct RobotConferenceData {
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
