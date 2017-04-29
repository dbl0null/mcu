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
using v8::MaybeLocal;

Persistent<Function> Stream::constructor;

Stream::Stream(double value) : value_(value) {
}

Stream::~Stream() {
}

void Stream::Init(Isolate* isolate) {
    std::cout << "Stream::Init" << std::endl;
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Stream"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

    constructor.Reset(isolate, tpl->GetFunction());
}

void Stream::New(const FunctionCallbackInfo<Value>& args) {
    std::cout << "Stream::New" << std::endl;
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
      // Invoked as constructor: `new MyObject(...)`
      double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
      MyObject* obj = new MyObject(value);
      obj->Wrap(args.This());
      args.GetReturnValue().Set(args.This());
    } else {
      // Invoked as plain function `MyObject(...)`, turn into construct call.
      const int argc = 1;
      Local<Value> argv[argc] = { args[0] };
      Local<Function> cons = Local<Function>::New(isolate, constructor);
      Local<Context> context = isolate->GetCurrentContext();
      Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();
      args.GetReturnValue().Set(instance);
    }
}

void Stream::NewInstance(const FunctionCallbackInfo<Value>& args) {
    std::cout << "Stream::NewInstance" << std::endl;
    Isolate* isolate = args.GetIsolate();

    const unsigned argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> instance = cons->NewInstance(context, argc, argv).ToLocalChecked();

    args.GetReturnValue().Set(instance);
}

void Stream::PlusOne(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
