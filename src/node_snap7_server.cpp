/*
* Copyright (c) 2014, Mathias Küsel
* MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
*/

#include "./node_snap7_server.h"
#include "nan.h"
#include <node_buffer.h>


namespace node_snap7{

	v8::Persistent<v8::FunctionTemplate> S7Server::constructor;

	void S7Server::Init(v8::Handle<v8::Object> exports) {
		NanScope();

		v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(S7Server::New);

		v8::Local<v8::String> name = NanNew<v8::String>("S7Server");
		tpl->SetClassName(name);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Setup the prototype
		// Administrative functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "Start", S7Server::Start);
		NODE_SET_PROTOTYPE_METHOD(tpl, "StartTo", S7Server::StartTo);
		NODE_SET_PROTOTYPE_METHOD(tpl, "Stop", S7Server::Stop);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetParam", S7Server::GetParam);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetParam", S7Server::SetParam);
		// Shared memory functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "RegisterArea", S7Server::RegisterArea);
		NODE_SET_PROTOTYPE_METHOD(tpl, "UnregisterArea", S7Server::UnregisterArea);
		NODE_SET_PROTOTYPE_METHOD(tpl, "LockArea", S7Server::LockArea);
		NODE_SET_PROTOTYPE_METHOD(tpl, "UnlockArea", S7Server::UnlockArea);
		// Control flow functions
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetEventsCallback", S7Server::SetEventsCallback);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetReadsEventsCallback", S7Server::SetReadsEventsCallback);
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetMask", S7Server::GetMask);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetMask", S7Server::SetMask);
		NODE_SET_PROTOTYPE_METHOD(tpl, "PickEvent", S7Server::PickEvent);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ClearEvents", S7Server::ClearEvents);
		// Miscellaneous function
		NODE_SET_PROTOTYPE_METHOD(tpl, "GetStatus", S7Server::GetStatus);
		NODE_SET_PROTOTYPE_METHOD(tpl, "SetCpuStatus", S7Server::SetCpuStatus);
		NODE_SET_PROTOTYPE_METHOD(tpl, "EventText", S7Server::EventText);
		NODE_SET_PROTOTYPE_METHOD(tpl, "ErrorText", S7Server::ErrorText);
		// Error codes
		NanSetPrototypeTemplate(tpl, "errSrvCannotStart", NanNew<v8::Integer>(errSrvCannotStart));
		NanSetPrototypeTemplate(tpl, "errSrvDBNullPointer", NanNew<v8::Integer>(errSrvDBNullPointer));
		NanSetPrototypeTemplate(tpl, "errSrvAreaAlreadyExists", NanNew<v8::Integer>(errSrvAreaAlreadyExists));
		NanSetPrototypeTemplate(tpl, "errSrvUnknownArea", NanNew<v8::Integer>(errSrvUnknownArea));
		NanSetPrototypeTemplate(tpl, "errSrvInvalidParams", NanNew<v8::Integer>(errSrvInvalidParams));
		NanSetPrototypeTemplate(tpl, "errSrvTooManyDB", NanNew<v8::Integer>(errSrvTooManyDB));
		NanSetPrototypeTemplate(tpl, "errSrvInvalidParamNumber", NanNew<v8::Integer>(errSrvInvalidParamNumber));
		NanSetPrototypeTemplate(tpl, "errSrvCannotChangeParam", NanNew<v8::Integer>(errSrvCannotChangeParam));
		// Area ID
		NanSetPrototypeTemplate(tpl, "srvAreaPE", NanNew<v8::Integer>(srvAreaPE));
		NanSetPrototypeTemplate(tpl, "srvAreaPA", NanNew<v8::Integer>(srvAreaPA));
		NanSetPrototypeTemplate(tpl, "srvAreaMK", NanNew<v8::Integer>(srvAreaMK));
		NanSetPrototypeTemplate(tpl, "srvAreaCT", NanNew<v8::Integer>(srvAreaCT));
		NanSetPrototypeTemplate(tpl, "srvAreaTM", NanNew<v8::Integer>(srvAreaTM));
		NanSetPrototypeTemplate(tpl, "srvAreaDB", NanNew<v8::Integer>(srvAreaDB));
		// TCP Server Event codes
		NanSetPrototypeTemplate(tpl, "evcServerStarted", NanNew<v8::Integer>(evcServerStarted));
		NanSetPrototypeTemplate(tpl, "evcServerStopped", NanNew<v8::Integer>(evcServerStopped));
		NanSetPrototypeTemplate(tpl, "evcListenerCannotStart", NanNew<v8::Integer>(evcListenerCannotStart));
		NanSetPrototypeTemplate(tpl, "evcClientAdded", NanNew<v8::Integer>(evcClientAdded));
		NanSetPrototypeTemplate(tpl, "evcClientRejected", NanNew<v8::Integer>(evcClientRejected));
		NanSetPrototypeTemplate(tpl, "evcClientNoRoom", NanNew<v8::Integer>(evcClientNoRoom));
		NanSetPrototypeTemplate(tpl, "evcClientException", NanNew<v8::Integer>(evcClientException));
		NanSetPrototypeTemplate(tpl, "evcClientDisconnected", NanNew<v8::Integer>(evcClientDisconnected));
		NanSetPrototypeTemplate(tpl, "evcClientTerminated", NanNew<v8::Integer>(evcClientTerminated));
		NanSetPrototypeTemplate(tpl, "evcClientsDropped", NanNew<v8::Integer>(evcClientsDropped));
		// S7 Server Event Code
		NanSetPrototypeTemplate(tpl, "evcPDUincoming", NanNew<v8::Integer>(evcPDUincoming));
		NanSetPrototypeTemplate(tpl, "evcDataRead", NanNew<v8::Integer>(evcDataRead));
		NanSetPrototypeTemplate(tpl, "evcDataWrite", NanNew<v8::Integer>(evcDataWrite));
		NanSetPrototypeTemplate(tpl, "evcNegotiatePDU", NanNew<v8::Integer>(evcNegotiatePDU));
		NanSetPrototypeTemplate(tpl, "evcReadSZL", NanNew<v8::Integer>(evcReadSZL));
		NanSetPrototypeTemplate(tpl, "evcClock", NanNew<v8::Integer>(evcClock));
		NanSetPrototypeTemplate(tpl, "evcUpload", NanNew<v8::Integer>(evcUpload));
		NanSetPrototypeTemplate(tpl, "evcDownload", NanNew<v8::Integer>(evcDownload));
		NanSetPrototypeTemplate(tpl, "evcDirectory", NanNew<v8::Integer>(evcDirectory));
		NanSetPrototypeTemplate(tpl, "evcSecurity", NanNew<v8::Integer>(evcSecurity));
		NanSetPrototypeTemplate(tpl, "evcControl", NanNew<v8::Integer>(evcControl));
		// Masks to enable/disable all events
		NanSetPrototypeTemplate(tpl, "evcAll", NanNew<v8::Integer>(evcAll));
		NanSetPrototypeTemplate(tpl, "evcNone", NanNew<v8::Integer>(evcNone));
		// Event SubCodes
		NanSetPrototypeTemplate(tpl, "evsUnknown", NanNew<v8::Integer>(evsUnknown));
		NanSetPrototypeTemplate(tpl, "evsStartUpload", NanNew<v8::Integer>(evsStartUpload));
		NanSetPrototypeTemplate(tpl, "evsStartDownload", NanNew<v8::Integer>(evsStartDownload));
		NanSetPrototypeTemplate(tpl, "evsGetBlockList", NanNew<v8::Integer>(evsGetBlockList));
		NanSetPrototypeTemplate(tpl, "evsStartListBoT", NanNew<v8::Integer>(evsStartListBoT));
		NanSetPrototypeTemplate(tpl, "evsListBoT", NanNew<v8::Integer>(evsListBoT));
		NanSetPrototypeTemplate(tpl, "evsGetBlockInfo", NanNew<v8::Integer>(evsGetBlockInfo));
		NanSetPrototypeTemplate(tpl, "evsGetClock", NanNew<v8::Integer>(evsGetClock));
		NanSetPrototypeTemplate(tpl, "evsSetClock", NanNew<v8::Integer>(evsSetClock));
		NanSetPrototypeTemplate(tpl, "evsSetPassword", NanNew<v8::Integer>(evsSetPassword));
		NanSetPrototypeTemplate(tpl, "evsClrPassword", NanNew<v8::Integer>(evsClrPassword));
		// Event Params : functions group
		NanSetPrototypeTemplate(tpl, "grProgrammer", NanNew<v8::Integer>(grProgrammer));
		NanSetPrototypeTemplate(tpl, "grCyclicData", NanNew<v8::Integer>(grCyclicData));
		NanSetPrototypeTemplate(tpl, "grBlocksInfo", NanNew<v8::Integer>(grBlocksInfo));
		NanSetPrototypeTemplate(tpl, "grSZL", NanNew<v8::Integer>(grSZL));
		NanSetPrototypeTemplate(tpl, "grPassword", NanNew<v8::Integer>(grPassword));
		NanSetPrototypeTemplate(tpl, "grBSend", NanNew<v8::Integer>(grBSend));
		NanSetPrototypeTemplate(tpl, "grClock", NanNew<v8::Integer>(grClock));
		NanSetPrototypeTemplate(tpl, "grSecurity", NanNew<v8::Integer>(grSecurity));
		// Event Params : control codes
		NanSetPrototypeTemplate(tpl, "CodeControlUnknown", NanNew<v8::Integer>(CodeControlUnknown));
		NanSetPrototypeTemplate(tpl, "CodeControlColdStart", NanNew<v8::Integer>(CodeControlColdStart));
		NanSetPrototypeTemplate(tpl, "CodeControlWarmStart", NanNew<v8::Integer>(CodeControlWarmStart));
		NanSetPrototypeTemplate(tpl, "CodeControlStop", NanNew<v8::Integer>(CodeControlStop));
		NanSetPrototypeTemplate(tpl, "CodeControlCompress", NanNew<v8::Integer>(CodeControlCompress));
		NanSetPrototypeTemplate(tpl, "CodeControlCpyRamRom", NanNew<v8::Integer>(CodeControlCpyRamRom));
		NanSetPrototypeTemplate(tpl, "CodeControlInsDel", NanNew<v8::Integer>(CodeControlInsDel));
		// Event Result
		NanSetPrototypeTemplate(tpl, "evrNoError", NanNew<v8::Integer>(evrNoError));
		NanSetPrototypeTemplate(tpl, "evrFragmentRejected", NanNew<v8::Integer>(evrFragmentRejected));
		NanSetPrototypeTemplate(tpl, "evrMalformedPDU", NanNew<v8::Integer>(evrMalformedPDU));
		NanSetPrototypeTemplate(tpl, "evrSparseBytes", NanNew<v8::Integer>(evrSparseBytes));
		NanSetPrototypeTemplate(tpl, "evrCannotHandlePDU", NanNew<v8::Integer>(evrCannotHandlePDU));
		NanSetPrototypeTemplate(tpl, "evrNotImplemented", NanNew<v8::Integer>(evrNotImplemented));
		NanSetPrototypeTemplate(tpl, "evrErrException", NanNew<v8::Integer>(evrErrException));
		NanSetPrototypeTemplate(tpl, "evrErrAreaNotFound", NanNew<v8::Integer>(evrErrAreaNotFound));
		NanSetPrototypeTemplate(tpl, "evrErrOutOfRange", NanNew<v8::Integer>(evrErrOutOfRange));
		NanSetPrototypeTemplate(tpl, "evrErrOverPDU", NanNew<v8::Integer>(evrErrOverPDU));
		NanSetPrototypeTemplate(tpl, "evrErrTransportSize", NanNew<v8::Integer>(evrErrTransportSize));
		NanSetPrototypeTemplate(tpl, "evrInvalidGroupUData", NanNew<v8::Integer>(evrInvalidGroupUData));
		NanSetPrototypeTemplate(tpl, "evrInvalidSZL", NanNew<v8::Integer>(evrInvalidSZL));
		NanSetPrototypeTemplate(tpl, "evrDataSizeMismatch", NanNew<v8::Integer>(evrDataSizeMismatch));
		NanSetPrototypeTemplate(tpl, "evrCannotUpload", NanNew<v8::Integer>(evrCannotUpload));
		NanSetPrototypeTemplate(tpl, "evrCannotDownload", NanNew<v8::Integer>(evrCannotDownload));
		NanSetPrototypeTemplate(tpl, "evrUploadInvalidID", NanNew<v8::Integer>(evrUploadInvalidID));
		NanSetPrototypeTemplate(tpl, "evrResNotFound", NanNew<v8::Integer>(evrResNotFound));

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
		}
		else{
			v8::Local<v8::FunctionTemplate> constructorHandle = NanNew<v8::FunctionTemplate>(constructor);
			NanReturnValue(constructorHandle->GetFunction()->NewInstance());
		}
	}

	S7Server::~S7Server(){
		delete snap7Server;
		NanDisposePersistent(constructor);
		uv_mutex_destroy(&mutex);
	}

	// Control functions
	void StartWorker::Execute(){
		if (strcmp(address, "") == 0)
			returnValue = s7server->snap7Server->Start();
		else
			returnValue = s7server->snap7Server->StartTo(address);

		delete[] address;
	}

	void StartWorker::HandleOKCallback(){
		NanScope();

		v8::Local<v8::Value> argv[1];
		if (returnValue == 0)
			argv[0] = NanNull();
		else
			argv[0] = NanNew<v8::Integer>(returnValue);

		callback->Call(1, argv);
	}

	NAN_METHOD(S7Server::Start){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		if (!args[0]->IsFunction()){
			NanReturnValue(NanNew<v8::Boolean>(s7server->snap7Server->Start() == 0));
		}
		else{
			char* address = new char(0);
			NanCallback *callback = new NanCallback(args[0].As<v8::Function>());
			NanAsyncQueueWorker(new StartWorker(callback, s7server, address));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Server::StartTo){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		if (args.Length() < 1) {
			return NanThrowTypeError("Wrong number of arguments");
		}

		if (!args[0]->IsString()) {
			return NanThrowTypeError("Wrong arguments");
		}

		size_t len;
		char* remAddress = NanCString(args[0], &len);

		if (!args[3]->IsFunction()){
			v8::Local<v8::Boolean> ret = NanNew<v8::Boolean>(s7server->snap7Server->StartTo(remAddress) == 0);
			delete[] remAddress;
			NanReturnValue(ret);
		}
		else{
			NanCallback *callback = new NanCallback(args[1].As<v8::Function>());
			NanAsyncQueueWorker(new StartWorker(callback, s7server, remAddress));
			NanReturnUndefined();
		}
	}

	NAN_METHOD(S7Server::Stop){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnValue(NanNew<v8::Boolean>(s7server->snap7Server->Stop() == 0));
	}

	NAN_METHOD(S7Server::GetParam){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		if (!args[0]->IsNumber() || !args[2]->IsObject()) {
			return NanThrowTypeError("Wrong arguments");
		}

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::SetParam){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::RegisterArea){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::UnregisterArea){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::LockArea){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::UnlockArea){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::SetEventsCallback){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::SetReadsEventsCallback){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::GetMask){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::SetMask){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::PickEvent){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::ClearEvents){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::GetStatus){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::SetCpuStatus){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::EventText){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

	NAN_METHOD(S7Server::ErrorText){
		NanScope();
		S7Server *s7server = ObjectWrap::Unwrap<S7Server>(args.This());

		NanReturnUndefined();
	}

}