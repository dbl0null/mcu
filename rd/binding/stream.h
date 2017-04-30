#ifndef STREAM_H
#define STREAM_H

#include <node.h>
#include <node_object_wrap.h>

namespace gstream {

    class Stream : public node::ObjectWrap {
        public:
            static void Init(v8::Isolate* isolate);
            static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
        private:
         explicit Stream();
         ~Stream();
         static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
         static void AddRobot(const v8::FunctionCallbackInfo<v8::Value>& args);
         static v8::Persistent<v8::Function> constructor;
         std::string _conferenceId;
         std::string _defaultView;
    };

}

#endif
