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

    Persistent<Function> Stream::constructor;

    Stream::Stream(double value) : value_(value) {
    }

    Stream::~Stream() {
    }

    void Stream::Init(Isolate* isolate) {
        std::cout << "Stream::Init" << std::endl;
        Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
        tpl->SetClassName(String::NewFromUtf8(isolate, "Stream"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

        constructor.Reset(isolate, tpl->GetFunction());
    }

    void Stream::New(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Stream::New" << std::endl;
        Isolate* isolate = args.GetIsolate();

        if (args.IsConstructCall()) {
            // Invoked as constructor: `new Stream(...)`
            double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
            Stream* obj = new Stream(value);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // Invoked as plain function `Stream(...)`, turn into construct call.
            const int argc = 1;
            Local<Value> argv[argc] = { args[0] };
            Local<Function> cons = Local<Function>::New(isolate, constructor);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
            args.GetReturnValue().Set(instance);
        }
    }

    void Stream::NewInstance(const FunctionCallbackInfo<Value>& args) {
        std::cout << "Stream::NewInstance" << std::endl;
        Isolate* isolate = args.GetIsolate();
        const unsigned argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        Local<Context> context = isolate->GetCurrentContext();
        Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
        args.GetReturnValue().Set(instance);
    }

    void Stream::PlusOne(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();

        Stream* obj = ObjectWrap::Unwrap<Stream>(args.Holder());
        obj->value_ += 1;

        args.GetReturnValue().Set(Number::New(isolate, obj->value_));
    }

}
