#include <iostream>
#include "stream.h"

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

Persistent<Function> Stream::constructor;

Stream::Stream(Local<v8::String> conferenceId) : conferenceId_(conferenceId) {
}

Stream::~Stream() {
}

void Stream::Init(Local<Object> exports) {
    std::cout << "Stream::Init" << std::endl;
    Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Stream"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "addRobot", AddRobot);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Stream"), tpl->GetFunction());
}

void Stream::New(const FunctionCallbackInfo<Value>& args) {
    std::cout << "Stream::New" << std::endl;
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Invoked as constructor: `new Stream(...)`
        Local<v8::String> conferenceId = args[0]->ToString();
        Stream* obj = new Stream(conferenceId);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        // Invoked as plain function `Stream(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Context> context = isolate->GetCurrentContext();
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }
}

void Stream::AddRobot(const FunctionCallbackInfo<Value>& args) {
    std::cout << "Stream::AddRobot" << std::endl;
    Isolate* isolate = args.GetIsolate();
    Stream* stream = ObjectWrap::Unwrap<Stream>(args.Holder());

    // callback results
    Local<Function> cb = Local<Function>::Cast(args[1]);
    Local<Value> err = v8::Null(isolate);
    Local<Value> robot = v8::Null(isolate);
    const unsigned argc = 2; // number of returned parameters
    Local<Value> argv[argc] = { err, robot };
    cb->Call(Null(isolate), argc, argv);
}
