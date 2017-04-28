#ifndef STREAM_H
#define STREAM_H

#include <nan.h>

class Stream : public Nan::ObjectWrap {
 public:
  static void Init();
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  String::Utf8Value conferenceId() const { return conferenceId_; }
  String::Utf8Value defaultView() const { return defaultView_; }

 private:
  Stream();
  ~Stream();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  String::Utf8Value conferenceId_;
  String::Utf8Value defaultView_;
};

#endif
