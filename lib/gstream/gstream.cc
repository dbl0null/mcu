#include <nan.h>
#include "stream.h"

using namespace v8;

void CreateStream(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(Stream::NewInstance(info[0]));
}

void Add(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  Stream* obj1 = Nan::ObjectWrap::Unwrap<Stream>(info[0]->ToObject());
  Stream* obj2 = Nan::ObjectWrap::Unwrap<Stream>(info[1]->ToObject());
  double sum = obj1->Val() + obj2->Val();
  info.GetReturnValue().Set(Nan::New(sum));
}

void InitAll(v8::Local<v8::Object> exports) {
  Stream::Init();

  exports->Set(Nan::New("createObject").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(CreateStream)->GetFunction());

  exports->Set(Nan::New("add").ToLocalChecked(),
      Nan::New<v8::FunctionTemplate>(Add)->GetFunction());
}

NODE_MODULE(gstream, InitAll)
