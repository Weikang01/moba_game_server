#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../lua_wrapper/lua_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <tolua++.h>
#ifdef __cplusplus
}
#endif

#include <tolua_fix.h>

#include "proto_manager_export_to_lua.h"
#include "../netbus/proto_manager.h"

static int lua_protomanager_init(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    int proto_type;
    if (argc != 1)
		goto failed;

    proto_type = lua_tointeger(tolua_S, 1);
    if (proto_type != PROTO_JSON && proto_type != PROTO_BUF)
        goto failed;

    ProtoManager::init(proto_type);
failed:
    return 1;
}

static int lua_proto_type(lua_State* tolua_S)
{
    lua_pushinteger(tolua_S, ProtoManager::proto_type());
    return 1;
}

static int lua_register_protobuf_cmd_map(lua_State* tolua_S)
{
    /*
    input
    lua table:
    {"cmd1", "cmd2", ...}
	*/

    std::map<int, std::string> cmd_map;
    int tsize = luaL_len(tolua_S, 1);
    if (tsize <= 0)
		goto failed;

    for (int i = 1; i <= tsize; i++)
    {
		lua_pushinteger(tolua_S, i);
		lua_gettable(tolua_S, 1);
		const char* cmd = luaL_checkstring(tolua_S, -1);
		if (cmd == NULL)
			goto failed;
		cmd_map[i] = cmd;
		lua_pop(tolua_S, 1);
	}

    ProtoManager::register_protobuf_cmd_map(cmd_map);

    lua_pushboolean(tolua_S, 1);
    return 1;
failed:
    lua_pushboolean(tolua_S, 0);
    return 0;
}


int register_proto_manager_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "ProtoManager", 0);
        tolua_beginmodule(tolua_S, "ProtoManager");

        tolua_function(tolua_S, "init", lua_protomanager_init);
        tolua_function(tolua_S, "proto_type", lua_proto_type);
        tolua_function(tolua_S, "register_protobuf_cmd_map", lua_register_protobuf_cmd_map);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
