/*
* Copyright (c) 2014, Mathias K�sel
* MIT +no-false-attribs License <https://github.com/mathiask88/node-snap7/blob/master/LICENSE>
*/

#include "node_snap7_client.h"
#include "node_snap7_server.h"

extern "C"{
	void InitAll(v8::Handle<v8::Object> exports) {
		node_snap7::S7Client::Init(exports);
		node_snap7::S7Server::Init(exports);
	}
}

NODE_MODULE(node_snap7, InitAll)
