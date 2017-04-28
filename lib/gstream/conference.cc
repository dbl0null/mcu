#include <node.h>
#include "conference.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using v8::Local;
using v8::Number;
using v8::Value;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;
using Nan::Callback;
using Nan::HandleScope;
using Nan::Null;
using Nan::To;



Nan::Persistent<v8::Function> Conference::constructor;

void Conference::Init() {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Conference").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(tpl->GetFunction());
}

void Conference::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Conference* obj = new Conference();
  obj->conferenceId_ = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> Conference::NewInstance(v8::Local<v8::Value> arg) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 1;
  v8::Local<v8::Value> argv[argc] = { arg };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}