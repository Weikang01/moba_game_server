#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pf_cmd_map.h"

#include <string>
#include <map>
#include "../../netbus/proto_manager.h"


std::map<int, std::string> cmd_map = {
	{ 0, "LoginReq" },
	{ 1, "LoginRes" }
};

void pf_cmd_map_init()
{
	ProtoManager::register_protobuf_cmd_map(cmd_map);
}
