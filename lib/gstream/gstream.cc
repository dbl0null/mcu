#include <nan.h>

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

NAN_METHOD(CreateStream) {
    // parse info[0] as conference object
  Callback *callback = new Callback(info[1].As<Function>());

  Local<Value> argv[] = {
      Null() // err is null
    , New<Number>(25) // return object
  };
  callback->Call(2, argv);
}

NAN_MODULE_INIT(InitAll) {

  Set(target, New<String>("createStream").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(CreateStream)).ToLocalChecked());
}

NODE_MODULE(gstream, InitAll);
