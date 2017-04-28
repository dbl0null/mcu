#include <iostream>
#include <node.h>
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
using v8::Array;
using v8::Exception;
using v8::Null;

// void CreateStream(const FunctionCallbackInfo<Value>& args) {
//     Isolate* isolate = args.GetIsolate();
//
//     if(args.Length() < 1 || !args[0]->IsObject()) {
//         isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error: One object expected")));
//         return;
//     }
//
//     Local<Context> context = isolate->GetCurrentContext();
//     Local<Object> obj = args[0]->ToObject(context).ToLocalChecked();
//     Local<Value> localVal = obj->Get("conferenceId").ToLocalChecked();
//     std::string val = *String::Utf8Value(localVal);
//     std::cout << val << std::endl;
//
//     Callback *callback = new Callback(args[1].As<Function>());
//     Local<Value> argv[] = { // callback parameters
//         Null(), // err is null
//         conferenceId // return object
//     };
//     callback->Call(2, argv);
// }

void CreateStream(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    // basic validation
    if(args.Length() != 2) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error: 2 parameters expected")));
        return;
    }
    if(!args[0]->IsObject()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Error: First parameter must be an object for options")));
        return;
    }

    // parse options
    // Local<Context> context = isolate->GetCurrentContext();
    // Local<Object> options = args[0]->ToObject(context).ToLocalChecked();
    // Local<Value> localVal = options->conferenceId;
    // std::string val = *String::Utf8Value(localVal);
    // std::cout << val << std::endl;

    // create return object
    // Local<Object> stream = Object::New(isolate);
    // stream->Set(String::NewFromUtf8(isolate, "conferenceId"), String::NewFromUtf8(isolate, "asdf"));
    Stream* obj = new Stream(String::NewFromUtf8(isolate, "c1"));

    // callback results
    Local<Function> cb = Local<Function>::Cast(args[1]);
    Local<Value> err = v8::Null(isolate);
    Local<Value> gstream = stream;
    const unsigned argc = 2; // number of returned parameters
    Local<Value> argv[argc] = { err, gstream };
    cb->Call(Null(isolate), argc, argv);
}

void Init(Local<Object> exports) {
    std::cout << "Init" << std::endl;
    NODE_SET_METHOD(exports, "createStream", CreateStream);
    Stream::Init(exports);
}

NODE_MODULE(gstream, Init);
