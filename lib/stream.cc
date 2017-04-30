#include <iostream>
#include <node.h>
#include "stream.h"
#include "streamdata.h"

namespace gstream {

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

    Persistent<Function> Stream::constructor;

    Stream::Stream() {
    }

    Stream::~Stream() {
    }

    void Stream::Init(Isolate* isolate) {
        std::cout << "Stream::Init" << std::endl;
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        tpl->SetClassName(String::NewFromUtf8(isolate, "Stream"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(tpl, "addRobot", AddRobot);

        constructor.Reset(isolate, tpl->GetFunction());
    }

    void Stream::New(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Stream::New" << std::endl;
        Isolate* isolate = args.GetIsolate();
        Local<Context> context = isolate->GetCurrentContext();

        if (args.IsConstructCall()) {
            std::cout << "Stream::New: new ()" << std::endl;
            // Invoked as constructor: `new Stream(...)`
            Local<Object> options = args[0]->ToObject(context).ToLocalChecked();
            Local<Value> conferenceId = options->Get(context, String::NewFromUtf8(isolate, "conferenceId")).ToLocalChecked();
            Local<Value> defaultView = options->Get(context, String::NewFromUtf8(isolate, "defaultView")).ToLocalChecked();
            Stream* stream = new Stream();
            stream->robotStreamData.ConferenceId = *String::Utf8Value(conferenceId);
            stream->robotStreamData.DefaultView = *String::Utf8Value(defaultView);
            // std::cout << stream->_conferenceId << std::endl;
            stream->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            std::cout << "Stream::New: ()" << std::endl;
            // Invoked as plain function `Stream(...)`, turn into construct call.
            Local<Function> cons = Local<Function>::New(isolate, constructor);
            Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
            args.GetReturnValue().Set(instance);
        }
    }

    void Stream::NewInstance(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Stream::NewInstance" << std::endl;
        Isolate* isolate = args.GetIsolate();
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Context> context = isolate->GetCurrentContext();

        const unsigned argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }

    void Stream::AddRobot(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Stream::AddRobot" << std::endl;
        Isolate* isolate = args.GetIsolate();
        Stream* stream = ObjectWrap::Unwrap<Stream>(args.Holder());
        Local<Function> cb = Local<Function>::Cast(args[1]);
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> robot = args[0]->ToObject(context).ToLocalChecked();
        // Local<Value> robotId = robot->Get(context, String::NewFromUtf8(isolate, "robot_id")).ToLocalChecked();
        Local<Value> image = robot->Get(context, String::NewFromUtf8(isolate, "image")).ToLocalChecked();

        if(!EnsurePipeline(stream)){
            std::cout << "Stream::AddRobot::EnsurePipeline failed" << std::endl;
        }

        if(!SetupRobot(stream, *String::Utf8Value(image))){
            std::cout << "Stream::AddRobot::SetupRobot failed" << std::endl;
        }

        // play robot video
        GstStateChangeReturn ret = gst_element_set_state (stream->robotStreamData.Pipeline, GST_STATE_PLAYING);
        if (ret == GST_STATE_CHANGE_FAILURE) {
            std::cout << "Stream::AddRobot::PlayPipeline failed" << std::endl;
        }

        // callback results
        Local<Value> err = v8::Null(isolate);
        Local<Value> argv[1] = { err };
        cb->Call(Null(isolate), 1, argv);
    }

    bool EnsurePipeline(Stream* stream){
        if(stream->robotStreamData.Pipeline){
            return true;
        }else{
            gst_init(NULL, NULL);
            stream->robotStreamData.Pipeline = gst_pipeline_new(stream->robotStreamData.ConferenceId.c_str());
            if (stream->robotStreamData.Pipeline) {
                return true;
            } else {
                return false;
            }
        }
    }

    bool SetupRobot(Stream* stream, std::string image){
        GstElement *pipeline = stream->robotStreamData.Pipeline;


        // creating elements
        stream->robotStreamData.ImageSource = gst_element_factory_make("multifilesrc", "imagesource");
        if (!stream->robotStreamData.ImageSource) {
            return false;
        }
        if(!gst_bin_add(GST_BIN(pipeline), stream->robotStreamData.ImageSource)){
            return false;
        }

        stream->robotStreamData.JpegDec = gst_element_factory_make("jpegdec", "decoder");
        if (!stream->robotStreamData.JpegDec) {
            return false;
        }
        if(!gst_bin_add(GST_BIN(pipeline), stream->robotStreamData.JpegDec)){
            return false;
        }

        stream->robotStreamData.VideoConvert  = gst_element_factory_make("videoconvert", "videoconvert");
        if (!stream->robotStreamData.VideoConvert) {
            return false;
        }
        if(!gst_bin_add(GST_BIN(pipeline), stream->robotStreamData.VideoConvert)){
            return false;
        }

        stream->robotStreamData.AutoVideoSink = gst_element_factory_make("autovideosink", "videosink");
        if (!stream->robotStreamData.AutoVideoSink) {
            return false;
        }
        if(!gst_bin_add(GST_BIN(pipeline), stream->robotStreamData.AutoVideoSink)){
            return false;
        }

        // linking elements
        if(!gst_element_link(stream->robotStreamData.ImageSource, stream->robotStreamData.JpegDec)){
            return false;
        }

        if(!gst_element_link(stream->robotStreamData.JpegDec, stream->robotStreamData.VideoConvert)){
            return false;
        }

        if(!gst_element_link(stream->robotStreamData.VideoConvert, stream->robotStreamData.AutoVideoSink)){
            return false;
        }

        // setup robot image
        g_object_set(G_OBJECT(stream->robotStreamData.ImageSource), "location", image.c_str(), NULL);

        return true;
    }
}
