#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class Conference : public Nan::ObjectWrap {
 public:
  static void Init();
  static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> arg);
  double conferenceId() const { return conferenceId_; }
  double conferenceId() const { return conferenceId_; }

 private:
  Conference();
  ~Conference();

  static Nan::Persistent<v8::Function> constructor;
  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
  double conferenceId_;
};

#endif
