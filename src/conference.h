#ifndef STREAM_H
#define STREAM_H

#include <node.h>
#include <node_object_wrap.h>
#include <gst/gst.h>
#include "conferencedata.h"

namespace mcu {

    class Conference : public node::ObjectWrap {
        public:
            static void Init(v8::Isolate* isolate);
            static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
            static void AddRobot(const v8::FunctionCallbackInfo<v8::Value>& args);
            static void AddWebRTCParticipant(const v8::FunctionCallbackInfo<v8::Value>& args);
            ConferenceData conferenceData;
        private:
            explicit Conference();
            ~Conference();
            static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
            static void EnsurePipeline(Conference *conference);
            static void EnsureElements(Conference *conference);
            static v8::Persistent<v8::Function> constructor;
    };

}

#endif
