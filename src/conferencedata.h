#ifndef CONFERENCEDATA_H
#define CONFERENCEDATA_H

#include <gst/gst.h>

namespace mcu {

    struct ConferenceData {
        std::string ConferenceId;
        std::string DefaultView;
        GstElement *Pipeline;
        GstBus *Bus;
    };

    struct RobotData {
        std::string RobotId;
        GstElement *ImageSource;
        GstElement *JpegDec;
        GstElement *VideoConvert;
        GstElement *AutoVideoSink;
    };

    struct WebRTCParticipantData {
        std::string ParticipantId;
        GstElement *ImageSource;
        GstElement *JpegDec;
        GstElement *VideoConvert;
        GstElement *AutoVideoSink;
    };
}

#endif
