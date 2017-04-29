#include <iostream>
#include <node.h>
#include "stream.h"

namespace gstream {

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void CreateStream(const FunctionCallbackInfo<Value>& args) {
        std::cout << "CreateStream" << std::endl;
        Stream::NewInstance(args);
    }

    void InitAll(Local<Object> exports, Local<Object> module) {
        std::cout << "Init" << std::endl;
        Stream::Init(exports->GetIsolate());
        NODE_SET_METHOD(module, "exports", CreateStream);
    }

    NODE_MODULE(gstream, InitAll)

}
