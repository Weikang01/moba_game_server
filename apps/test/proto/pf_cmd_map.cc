#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pf_cmd_map.h"

#include "../../netbus/proto_manager.h"

const char* pf_cmd_map[] = {
	"LoginReq",
	"LoginRes"
};

void pf_cmd_map_init()
{
	proto_manager::reg_cmd_map(pf_cmd_map, sizeof(pf_cmd_map) / sizeof(pf_cmd_map[0]));
}
