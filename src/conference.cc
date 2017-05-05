#include <iostream>
#include <node.h>
#include "conference.h"
#include "conferencedata.h"

namespace mcu {

    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Value;
    using v8::Exception;

    Persistent<Function> Conference::constructor;

    Conference::Conference() {
    }

    Conference::~Conference() {
    }

    void Conference::Init(Isolate* isolate) {
        std::cout << "Conference::Init" << std::endl;
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        tpl->SetClassName(String::NewFromUtf8(isolate, "Conference"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(tpl, "addRobot", AddRobot);
        NODE_SET_PROTOTYPE_METHOD(tpl, "addWebRTCParticipant", AddWebRTCParticipant);

        constructor.Reset(isolate, tpl->GetFunction());
    }

    void Conference::New(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Conference::New" << std::endl;
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall()) {
            std::cout << "Conference::New: new ()" << std::endl;
            // Invoked as constructor: `new Conference(...)`
            Local<Object> options = args[0]->ToObject(context).ToLocalChecked();
            Local<Value> conferenceId = options->Get(context, String::NewFromUtf8(isolate, "conferenceId")).ToLocalChecked();
            Local<Value> defaultView = options->Get(context, String::NewFromUtf8(isolate, "defaultView")).ToLocalChecked();
            Conference* conference = new Conference();
            conference->conferenceData.ConferenceId = *String::Utf8Value(conferenceId);
            conference->conferenceData.DefaultView = *String::Utf8Value(defaultView);
            // std::cout << conference->_conferenceId << std::endl;
            conference->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            std::cout << "Conference::New: ()" << std::endl;
            // Invoked as plain function `Conference(...)`, turn into construct call.
            Local<Function> cons = Local<Function>::New(isolate, constructor);
            Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
            args.GetReturnValue().Set(instance);
        }
    }

    void Conference::NewInstance(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Conference::NewInstance" << std::endl;
        Isolate* isolate = args.GetIsolate();
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Context> context = isolate->GetCurrentContext();

        const unsigned argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }

    void Conference::EnsurePipeline(Conference *conference) {
      std::cout << "Conference::EnsurePipeline" << std::endl;
      if(!conference->conferenceData.Pipeline){
          gst_init(NULL, NULL);
          conference->conferenceData.Pipeline = gst_pipeline_new(conference->conferenceData.ConferenceId.c_str());
          if (!conference->conferenceData.Pipeline) {
              std::cout << "Conference::AddRobot::EnsurePipeline failed" << std::endl;
          }
      }
    }

    void Conference::EnsureElements(Conference *conference) {
        std::cout << "Conference::EnsureElements" << std::endl;
        GstElement *pipeline = conference->conferenceData.Pipeline;

        if(!conference->conferenceData.VideoMixer){
            std::cout << "Conference::EnsureElements::Adding VideoMixer to pipeline" << std::endl;
            conference->conferenceData.VideoMixer = gst_element_factory_make("videomixer", "videomixer");
            gst_bin_add(GST_BIN(pipeline), conference->conferenceData.VideoMixer);
        }

        if(!conference->conferenceData.AudioMixer){
            std::cout << "Conference::EnsureElements::Adding AudioMixer to pipeline" << std::endl;
            conference->conferenceData.AudioMixer = gst_element_factory_make("audiomixer", "audiomixer");
            gst_bin_add(GST_BIN(pipeline), conference->conferenceData.AudioMixer);
        }
    }

    void Conference::AddRobot(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Conference::AddRobot" << std::endl;
        Isolate* isolate = args.GetIsolate();
        // Conference* conference = ObjectWrap::Unwrap<Conference>(args.Holder());
        Local<Function> cb = Local<Function>::Cast(args[1]);
        // Local<Context> context = isolate->GetCurrentContext();
        // Local<Object> robot = args[0]->ToObject(context).ToLocalChecked();
        // // Local<Value> robotId = robot->Get(context, String::NewFromUtf8(isolate, "robot_id")).ToLocalChecked();
        // Local<Value> image = robot->Get(context, String::NewFromUtf8(isolate, "image")).ToLocalChecked();

        // // ensure Pipeline
        // if(!conference->conferenceData.Pipeline){
        //     gst_init(NULL, NULL);
        //     conference->conferenceData.Pipeline = gst_pipeline_new(conference->conferenceData.ConferenceId.c_str());
        //     if (!conference->conferenceData.Pipeline) {
        //         std::cout << "Conference::AddRobot::EnsurePipeline failed" << std::endl;
        //     }
        // }
        //
        // // creating elements
        // GstElement *pipeline = conference->conferenceData.Pipeline;
        // conference->conferenceData.ImageSource = gst_element_factory_make("multifilesrc", "imagesource");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.ImageSource);
        //
        // conference->conferenceData.JpegDec = gst_element_factory_make("jpegdec", "decoder");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.JpegDec);
        //
        // conference->conferenceData.VideoConvert  = gst_element_factory_make("videoconvert", "videoconvert");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.VideoConvert);
        //
        // conference->conferenceData.AutoVideoSink = gst_element_factory_make("autovideosink", "videosink");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.AutoVideoSink);
        //
        // // linking elements
        // gst_element_link(conference->conferenceData.ImageSource, conference->conferenceData.JpegDec);
        // gst_element_link(conference->conferenceData.JpegDec, conference->conferenceData.VideoConvert);
        // gst_element_link(conference->conferenceData.VideoConvert, conference->conferenceData.AutoVideoSink);
        //
        // // setup robot images
        // std::string imglocation = *String::Utf8Value(image);
        // g_object_set(G_OBJECT(conference->conferenceData.ImageSource), "location", imglocation.c_str(), NULL);
        //
        // // play robot video
        // GstStateChangeReturn ret = gst_element_set_state (conference->conferenceData.Pipeline, GST_STATE_PLAYING);
        // if (ret == GST_STATE_CHANGE_FAILURE) {
        //     std::cout << "Conference::AddRobot::PlayPipeline failed" << std::endl;
        // }

        // callback results
        Local<Value> err = v8::Null(isolate);
        Local<Value> argv[1] = { err };
        cb->Call(Null(isolate), 1, argv);
    }

    void Conference::AddWebRTCParticipant(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Conference::AddWebRTCParticipant" << std::endl;
        Isolate* isolate = args.GetIsolate();

        // Unwrap conference option from node
        Conference* conference = ObjectWrap::Unwrap<Conference>(args.Holder());

        // get node options for addWebRTCParticipant
        Local<Function> cb = Local<Function>::Cast(args[1]);
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> participant = args[0]->ToObject(context).ToLocalChecked();
        Local<Value> participantId = participant->Get(context, String::NewFromUtf8(isolate, "participant_id")).ToLocalChecked();

        EnsurePipeline(conference);
        EnsureElements(conference);

        // // creating elements
        // GstElement *pipeline = conference->conferenceData.Pipeline;
        // conference->conferenceData.ImageSource = gst_element_factory_make("multifilesrc", "imagesource");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.ImageSource);
        //
        // conference->conferenceData.JpegDec = gst_element_factory_make("jpegdec", "decoder");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.JpegDec);
        //
        // conference->conferenceData.VideoConvert  = gst_element_factory_make("videoconvert", "videoconvert");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.VideoConvert);
        //
        // conference->conferenceData.AutoVideoSink = gst_element_factory_make("autovideosink", "videosink");
        // gst_bin_add(GST_BIN(pipeline), conference->conferenceData.AutoVideoSink);
        //
        // // linking elements
        // gst_element_link(conference->conferenceData.ImageSource, conference->conferenceData.JpegDec);
        // gst_element_link(conference->conferenceData.JpegDec, conference->conferenceData.VideoConvert);
        // gst_element_link(conference->conferenceData.VideoConvert, conference->conferenceData.AutoVideoSink);
        //
        // // setup robot images
        // std::string imglocation = *String::Utf8Value(image);
        // g_object_set(G_OBJECT(conference->conferenceData.ImageSource), "location", imglocation.c_str(), NULL);
        //
        // // play robot video
        // GstStateChangeReturn ret = gst_element_set_state (conference->conferenceData.Pipeline, GST_STATE_PLAYING);
        // if (ret == GST_STATE_CHANGE_FAILURE) {
        //     std::cout << "Conference::AddRobot::PlayPipeline failed" << std::endl;
        // }

        // callback results
        Local<Value> err = v8::Null(isolate);
        Local<Value> argv[1] = { err };
        cb->Call(Null(isolate), 1, argv);
    }
}
