#ifndef CONFERENCE_H
#define CONFERENCE_H

#include <nan.h>

class Conference : public Nan::ObjectWrap {
 public:
  static void Init();
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  v8::String conferenceId() const { return conferenceId_; }
  v8::String defaultView() const { return defaultView_; }

 private:
  Conference();
  ~Conference();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  v8::String conferenceId_;
  v8::String defaultView_;
};

#endif
