/* 
 * Copyright (c) 2014, Mathias Küsel
 * MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
 */

#ifndef SRC_NODE_SNAP7_SERVER_H_
#define SRC_NODE_SNAP7_SERVER_H_

#include <node.h>
#include <nan.h>
#include "./snap7.h"

namespace node_snap7 {

  class S7Server : public node::ObjectWrap {
   public:
    S7Server() {}
    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD(New);
    // Control functions
    static NAN_METHOD(Start);
    static NAN_METHOD(StartTo);
    static NAN_METHOD(Stop);
    static NAN_METHOD(GetParam);
    static NAN_METHOD(SetParam);
    // Shared memory functions
    static NAN_METHOD(RegisterArea);
    static NAN_METHOD(UnregisterArea);
    static NAN_METHOD(LockArea);
    static NAN_METHOD(UnlockArea);
    // Control flow functions
    static NAN_METHOD(SetEventsCallback);
    static NAN_METHOD(SetReadsEventsCallback);
    static NAN_METHOD(GetMask);
    static NAN_METHOD(SetMask);
    static NAN_METHOD(PickEvent);
    static NAN_METHOD(ClearEvents);
    // Miscellaneous function
    static NAN_METHOD(GetStatus);
    static NAN_METHOD(SetCpuStatus);
    static NAN_METHOD(EventText);
    static NAN_METHOD(ErrorText);

    uv_mutex_t mutex;
    TS7Server *snap7Server;

   private:
    ~S7Server();
    static v8::Persistent<v8::FunctionTemplate> constructor;
  };

  class StartWorker : public NanAsyncWorker {
   public:
    StartWorker(
      NanCallback *callback,
      S7Server *s7server,
      NanUtf8String *address)
      : NanAsyncWorker(callback), s7server(s7server), address(address) {}
    ~StartWorker() {}
    void Execute();
    void HandleOKCallback();

   private:
    S7Server *s7server;
    NanUtf8String *address;
    int returnValue;
  };

}  // namespace node_snap7

#endif  // SRC_NODE_SNAP7_SERVER_H_
