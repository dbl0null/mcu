#include <iostream>
#include <node.h>
#include "conference.h"

namespace mcu {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void CreateConference(const FunctionCallbackInfo<Value>& args) {
        std::cout << "CreateConference" << std::endl;
        Conference::NewInstance(args);
    }

    void InitAll(Local<Object> exports, Local<Object> module) {
        std::cout << "Init" << std::endl;
        Conference::Init(exports->GetIsolate());
        NODE_SET_METHOD(module, "exports", CreateConference);
    }

    NODE_MODULE(mcu, InitAll)

}
