#include <iostream>
#include <node.h>
#include "stream.h"

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
            stream->_conferenceId = *String::Utf8Value(conferenceId);
            stream->_defaultView = *String::Utf8Value(defaultView);
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

        if(args.Length() != 2) {
            Local<Value> argv[1] = { Exception::TypeError(String::NewFromUtf8(isolate, "Error: 2 parameters expected")) };
            cb->Call(Null(isolate), 1, argv);
            return;
        }
        if(!args[0]->IsObject()) {
            Local<Value> argv[1] = { Exception::TypeError(String::NewFromUtf8(isolate, "Error: First parameter must be an object for options")) };
            cb->Call(Null(isolate), 1, argv);
            return;
        }

        // do stuff here
        // obj->value_ += 1;

        // callback results
        Local<Value> err = v8::Null(isolate);
        Local<Value> value = Number::New(isolate, 10);
        Local<Value> argv[2] = { err, value };
        cb->Call(Null(isolate), 2, argv);
    }

}
