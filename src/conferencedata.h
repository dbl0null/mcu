#ifndef CONFERENCEDATA_H
#define CONFERENCEDATA_H

#include <gst/gst.h>

namespace mcu {

    struct RobotData {
        std::string RobotId;
        GstElement *ImageSource;
        GstElement *JpegDec;
        GstElement *VideoConvert;
    };

    struct WebRTCParticipantData {
        std::string ParticipantId;
        GstElement *NiceSource;
        GstElement *JpegDec;
        GstElement *VideoConvert;
        GstElement *NiceSink;
    };

    struct ConferenceData {
        std::string ConferenceId;
        std::string DefaultView;
        GstElement *Pipeline;
        GstElement *VideoMixer;
        GstElement *AudioMixer;
        GstBus *Bus;
        RobotData robots[1];
        WebRTCParticipantData webRTCParticipants[10];
    };
}

#endif
