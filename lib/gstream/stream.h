#ifndef STREAM_H
#define STREAM_H

#include <node.h>
#include <node_object_wrap.h>

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
using v8::Array;
using v8::Exception;
using v8::Null;

class Stream : public node::ObjectWrap {
    public:
        static void Init(v8::Local<v8::Object> exports);

    private:
        explicit Stream(Local<v8::String> conferenceId);
        ~Stream();

        static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
        static void AddRobot(const v8::FunctionCallbackInfo<v8::Value>& args);
        static v8::Persistent<v8::Function> constructor;
        Local<v8::String> conferenceId_;
};

#endif
