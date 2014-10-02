/*
* Copyright (c) 2014, Mathias Küsel
* MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
*/

#include "./node_snap7_server.h"
#include <node_buffer.h>


namespace node_snap7 {

  v8::Persistent<v8::FunctionTemplate> S7Server::constructor;

  void S7Server::Init(v8::Handle<v8::Object> exports) {
    NanScope();

    v8::Local<v8::FunctionTemplate> tpl;
    tpl = NanNew<v8::FunctionTemplate>(S7Server::New);

    v8::Local<v8::String> name = NanNew<v8::String>("S7Server");
    tpl->SetClassName(name);
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Setup the prototype
    // Administrative functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "Start"
      , S7Server::Start);
    NODE_SET_PROTOTYPE_METHOD(tpl, "StartTo"
      , S7Server::StartTo);
    NODE_SET_PROTOTYPE_METHOD(tpl, "Stop"
      , S7Server::Stop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "GetParam"
      , S7Server::GetParam);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetParam"
      , S7Server::SetParam);

    // Shared memory functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "RegisterArea"
      , S7Server::RegisterArea);
    NODE_SET_PROTOTYPE_METHOD(tpl, "UnregisterArea"
      , S7Server::UnregisterArea);
    NODE_SET_PROTOTYPE_METHOD(tpl, "LockArea"
      , S7Server::LockArea);
    NODE_SET_PROTOTYPE_METHOD(tpl, "UnlockArea"
      , S7Server::UnlockArea);

    // Control flow functions
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetEventsCallback"
      , S7Server::SetEventsCallback);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetReadsEventsCallback"
      , S7Server::SetReadsEventsCallback);
    NODE_SET_PROTOTYPE_METHOD(tpl, "GetMask"
      , S7Server::GetMask);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetMask"
      , S7Server::SetMask);
    NODE_SET_PROTOTYPE_METHOD(tpl, "PickEvent"
      , S7Server::PickEvent);
    NODE_SET_PROTOTYPE_METHOD(tpl, "ClearEvents"
      , S7Server::ClearEvents);

    // Miscellaneous function
    NODE_SET_PROTOTYPE_METHOD(tpl, "GetStatus"
      , S7Server::GetStatus);
    NODE_SET_PROTOTYPE_METHOD(tpl, "SetCpuStatus"
      , S7Server::SetCpuStatus);
    NODE_SET_PROTOTYPE_METHOD(tpl, "EventText"
      , S7Server::EventText);
    NODE_SET_PROTOTYPE_METHOD(tpl, "ErrorText"
      , S7Server::ErrorText);

    // Error codes
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvCannotStart")
      , NanNew<v8::Integer>(errSrvCannotStart), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvDBNullPointer")
      , NanNew<v8::Integer>(errSrvDBNullPointer), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvAreaAlreadyExists")
      , NanNew<v8::Integer>(errSrvAreaAlreadyExists), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvUnknownArea")
      , NanNew<v8::Integer>(errSrvUnknownArea), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvInvalidParams")
      , NanNew<v8::Integer>(errSrvInvalidParams), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvTooManyDB")
      , NanNew<v8::Integer>(errSrvTooManyDB), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvInvalidParamNumber")
      , NanNew<v8::Integer>(errSrvInvalidParamNumber), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("errSrvCannotChangeParam")
      , NanNew<v8::Integer>(errSrvCannotChangeParam), v8::ReadOnly);

    // Area ID
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaPE")
      , NanNew<v8::Integer>(srvAreaPE), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaPA")
      , NanNew<v8::Integer>(srvAreaPA), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaMK")
      , NanNew<v8::Integer>(srvAreaMK), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaCT")
      , NanNew<v8::Integer>(srvAreaCT), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaTM")
      , NanNew<v8::Integer>(srvAreaTM), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("srvAreaDB")
      , NanNew<v8::Integer>(srvAreaDB), v8::ReadOnly);

    // TCP Server Event codes
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcServerStarted")
      , NanNew<v8::Integer>(evcServerStarted), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcServerStopped")
      , NanNew<v8::Integer>(evcServerStopped), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcListenerCannotStart")
      , NanNew<v8::Integer>(evcListenerCannotStart), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientAdded")
      , NanNew<v8::Integer>(evcClientAdded), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientRejected")
      , NanNew<v8::Integer>(evcClientRejected), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientNoRoom")
      , NanNew<v8::Integer>(evcClientNoRoom), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientException")
      , NanNew<v8::Integer>(evcClientException), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientDisconnected")
      , NanNew<v8::Integer>(evcClientDisconnected), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientTerminated")
      , NanNew<v8::Integer>(evcClientTerminated), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClientsDropped")
      , NanNew<v8::Integer>(evcClientsDropped), v8::ReadOnly);

    // S7 Server Event Code
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcPDUincoming")
      , NanNew<v8::Integer>(evcPDUincoming), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcDataRead")
      , NanNew<v8::Integer>(evcDataRead), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcDataWrite")
      , NanNew<v8::Integer>(evcDataWrite), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcNegotiatePDU")
      , NanNew<v8::Integer>(evcNegotiatePDU), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcReadSZL")
      , NanNew<v8::Integer>(evcReadSZL), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcClock")
      , NanNew<v8::Integer>(evcClock), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcUpload")
      , NanNew<v8::Integer>(evcUpload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcDownload")
      , NanNew<v8::Integer>(evcDownload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcDirectory")
      , NanNew<v8::Integer>(evcDirectory), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcSecurity")
      , NanNew<v8::Integer>(evcSecurity), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcControl")
      , NanNew<v8::Integer>(evcControl), v8::ReadOnly);

    // Masks to enable/disable all events
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcAll")
      , NanNew<v8::Integer>(evcAll), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evcNone")
      , NanNew<v8::Integer>(evcNone), v8::ReadOnly);

    // Event SubCodes
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsUnknown")
      , NanNew<v8::Integer>(evsUnknown), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsStartUpload")
      , NanNew<v8::Integer>(evsStartUpload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsStartDownload")
      , NanNew<v8::Integer>(evsStartDownload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsGetBlockList")
      , NanNew<v8::Integer>(evsGetBlockList), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsStartListBoT")
      , NanNew<v8::Integer>(evsStartListBoT), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsListBoT")
      , NanNew<v8::Integer>(evsListBoT), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsGetBlockInfo")
      , NanNew<v8::Integer>(evsGetBlockInfo), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsGetClock")
      , NanNew<v8::Integer>(evsGetClock), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsSetClock")
      , NanNew<v8::Integer>(evsSetClock), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsSetPassword")
      , NanNew<v8::Integer>(evsSetPassword), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evsClrPassword")
      , NanNew<v8::Integer>(evsClrPassword), v8::ReadOnly);

    // Event Params : functions group
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grProgrammer")
      , NanNew<v8::Integer>(grProgrammer), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grCyclicData")
      , NanNew<v8::Integer>(grCyclicData), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grBlocksInfo")
      , NanNew<v8::Integer>(grBlocksInfo), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grSZL")
      , NanNew<v8::Integer>(grSZL), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grPassword")
      , NanNew<v8::Integer>(grPassword), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grBSend")
      , NanNew<v8::Integer>(grBSend), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grClock")
      , NanNew<v8::Integer>(grClock), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("grSecurity")
      , NanNew<v8::Integer>(grSecurity), v8::ReadOnly);

    // Event Params : control codes
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlUnknown")
      , NanNew<v8::Integer>(CodeControlUnknown), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlColdStart")
      , NanNew<v8::Integer>(CodeControlColdStart), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlWarmStart")
      , NanNew<v8::Integer>(CodeControlWarmStart), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlStop")
      , NanNew<v8::Integer>(CodeControlStop), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlCompress")
      , NanNew<v8::Integer>(CodeControlCompress), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlCpyRamRom")
      , NanNew<v8::Integer>(CodeControlCpyRamRom), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("CodeControlInsDel")
      , NanNew<v8::Integer>(CodeControlInsDel), v8::ReadOnly);

    // Event Result
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrNoError")
      , NanNew<v8::Integer>(evrNoError), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrFragmentRejected")
      , NanNew<v8::Integer>(evrFragmentRejected), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrMalformedPDU")
      , NanNew<v8::Integer>(evrMalformedPDU), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrSparseBytes")
      , NanNew<v8::Integer>(evrSparseBytes), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrCannotHandlePDU")
      , NanNew<v8::Integer>(evrCannotHandlePDU), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrNotImplemented")
      , NanNew<v8::Integer>(evrNotImplemented), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrErrException")
      , NanNew<v8::Integer>(evrErrException), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrErrAreaNotFound")
      , NanNew<v8::Integer>(evrErrAreaNotFound), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrErrOutOfRange")
      , NanNew<v8::Integer>(evrErrOutOfRange), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrErrOverPDU")
      , NanNew<v8::Integer>(evrErrOverPDU), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrErrTransportSize")
      , NanNew<v8::Integer>(evrErrTransportSize), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrInvalidGroupUData")
      , NanNew<v8::Integer>(evrInvalidGroupUData), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrInvalidSZL")
      , NanNew<v8::Integer>(evrInvalidSZL), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrDataSizeMismatch")
      , NanNew<v8::Integer>(evrDataSizeMismatch), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrCannotUpload")
      , NanNew<v8::Integer>(evrCannotUpload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrCannotDownload")
      , NanNew<v8::Integer>(evrCannotDownload), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrUploadInvalidID")
      , NanNew<v8::Integer>(evrUploadInvalidID), v8::ReadOnly);
    NanSetPrototypeTemplate(tpl
      , NanNew<v8::String>("evrResNotFound")
      , NanNew<v8::Integer>(evrResNotFound), v8::ReadOnly);

    NanAssignPersistent<v8::FunctionTemplate>(constructor, tpl);
    exports->Set(name, tpl->GetFunction());
  }

  NAN_METHOD(S7Server::New) {
    NanScope();
    if (args.IsConstructCall()) {
      S7Server *s7server = new S7Server();
      s7server->snap7Server = new TS7Server();
      uv_mutex_init(&s7server->mutex);

      s7server->Wrap(args.This());
      NanReturnValue(args.This());
    } else {
      v8::Local<v8::FunctionTemplate> constructorHandle;
      constructorHandle = NanNew<v8::FunctionTemplate>(constructor);
      NanReturnValue(constructorHandle->GetFunction()->NewInstance());
    }
  }

  S7Server::~S7Server() {
    delete snap7Server;
    NanDisposePersistent(constructor);
    uv_mutex_destroy(&mutex);
  }

  // Control functions
  void StartWorker::Execute() {
    if (strcmp(**address, "") == 0)
      returnValue = s7server->snap7Server->Start();
    else
      returnValue = s7server->snap7Server->StartTo(**address);

    delete address;
  }

  void StartWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[1];
    if (returnValue == 0)
      argv[0] = NanNull();
    else
      argv[0] = NanNew<v8::Integer>(returnValue);

    callback->Call(1, argv);
  }

  NAN_METHOD(S7Server::Start) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (!args[0]->IsFunction()) {
      NanReturnValue(NanNew<v8::Boolean>(s7server->snap7Server->Start() == 0));
    } else {
      NanUtf8String *address = new NanUtf8String(NanNew<v8::String>(""));
      NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
      NanAsyncQueueWorker(new StartWorker(callback, s7server, address));
      NanReturnUndefined();
    }
  }

  NAN_METHOD(S7Server::StartTo) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (args.Length() < 1) {
      return NanThrowTypeError("Wrong number of arguments");
    }

    if (!args[0]->IsString()) {
      return NanThrowTypeError("Wrong arguments");
    }

    NanUtf8String *locAddress = new NanUtf8String(args[0]);

    if (!args[1]->IsFunction()) {
      v8::Local<v8::Boolean> ret = NanNew<v8::Boolean>(
        s7server->snap7Server->StartTo(**locAddress) == 0);
      delete locAddress;
      NanReturnValue(ret);
    } else {
      NanCallback *callback = new NanCallback(args[1].As<v8::Function>());
      NanAsyncQueueWorker(new StartWorker(callback, s7server, locAddress));
      NanReturnUndefined();
    }
  }

  NAN_METHOD(S7Server::Stop) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnValue(NanNew<v8::Boolean>(s7server->snap7Server->Stop() == 0));
  }

  NAN_METHOD(S7Server::GetParam) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (!args[0]->IsNumber() || !args[2]->IsObject()) {
      return NanThrowTypeError("Wrong arguments");
    }

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::SetParam) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::RegisterArea) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (args.Length() < 3)
      return NanThrowTypeError("Wrong number of arguments");

    if (!args[0]->IsUint32() || !args[1]->IsUint32())
      return NanThrowTypeError("Wrong arguments");

    NanReturnValue(NanNew<v8::Boolean>(
      s7server->snap7Server->RegisterArea(args[0]->Int32Value(),
        args[1]->Uint32Value(),
        node::Buffer::Data(args[2].As<v8::Object>()),
        node::Buffer::Length(args[2].As<v8::Object>())) == 0));
  }

  NAN_METHOD(S7Server::UnregisterArea) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (args.Length() < 2)
      return NanThrowTypeError("Wrong number of arguments");

    if (!args[0]->IsUint32() || !args[1]->IsUint32())
      return NanThrowTypeError("Wrong arguments");

    NanReturnValue(NanNew<v8::Boolean>(
      s7server->snap7Server->UnregisterArea(args[0]->Int32Value(),
      args[1]->Uint32Value()) == 0));
  }

  NAN_METHOD(S7Server::LockArea) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (args.Length() < 2)
      return NanThrowTypeError("Wrong number of arguments");

    if (!args[0]->IsUint32() || !args[1]->IsUint32())
      return NanThrowTypeError("Wrong arguments");

    NanReturnValue(NanNew<v8::Boolean>(
      s7server->snap7Server->LockArea(args[0]->Int32Value(),
      args[1]->Uint32Value()) == 0));
  }

  NAN_METHOD(S7Server::UnlockArea) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (args.Length() < 2)
      return NanThrowTypeError("Wrong number of arguments");

    if (!args[0]->IsUint32() || !args[1]->IsUint32())
      return NanThrowTypeError("Wrong arguments");

    NanReturnValue(NanNew<v8::Boolean>(
      s7server->snap7Server->UnlockArea(args[0]->Int32Value(),
      args[1]->Uint32Value()) == 0));
  }

  NAN_METHOD(S7Server::SetEventsCallback) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::SetReadsEventsCallback) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::GetMask) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::SetMask) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::PickEvent) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::ClearEvents) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    s7server->snap7Server->ClearEvents();

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::GetStatus) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::SetCpuStatus) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::EventText) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    NanReturnUndefined();
  }

  NAN_METHOD(S7Server::ErrorText) {
    NanScope();
    S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

    if (!args[0]->IsNumber()) {
      return NanThrowTypeError("Wrong arguments");
    }

    NanReturnValue(NanNew<v8::String>(
      SrvErrorText(args[0]->Int32Value()).c_str()));
  }

}  // namespace node_snap7
