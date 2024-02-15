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

        // lua format: ProtoManager.init(PROTO_JSON or PROTO_BUF)
        // lua return: true or false
        tolua_function(tolua_S, "init", lua_protomanager_init);
        // lua format: ProtoManager.proto_type()
        // lua return: PROTO_JSON (int) or PROTO_BUF (int)
        tolua_function(tolua_S, "proto_type", lua_proto_type);
        // lua format: ProtoManager.register_protobuf_cmd_map({"cmd1", "cmd2", ...})
        // lua return: true or false
        tolua_function(tolua_S, "register_protobuf_cmd_map", lua_register_protobuf_cmd_map);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}

// in lua: RawCmd.read_header(raw_cmd)
// return: stype, ctype, utag
static int lua_raw_read_header(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    raw_cmd_msg* msg = NULL;
    if (argc != 1)
        goto failed;

    msg = (raw_cmd_msg*)tolua_touserdata(tolua_S, 1, NULL);
    if (msg == NULL)
        goto failed;

    lua_pushinteger(tolua_S, msg->stype);
    lua_pushinteger(tolua_S, msg->ctype);
    lua_pushinteger(tolua_S, msg->utag);

    return 3;
failed:
    return 0;
}

// in lua: RawCmd.set_utag(raw_cmd, utag)
// return: true or false
static int lua_raw_set_utag(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    raw_cmd_msg* raw = NULL;
    unsigned int utag;
    unsigned char* utag_ptr = NULL;
    if (argc != 2)
        goto failed;

    raw = (raw_cmd_msg*)tolua_touserdata(tolua_S, 1, NULL);

    if (raw == NULL)
		goto failed;

    utag = (unsigned int)luaL_checkinteger(tolua_S, 2);

    raw->utag = utag;

    utag_ptr = raw->raw_data + 4;
    utag_ptr[0] = (unsigned char)(utag & 0xff);
    utag_ptr[1] = (unsigned char)((utag >> 8) & 0xff);
    utag_ptr[2] = (unsigned char)((utag >> 16) & 0xff);
    utag_ptr[3] = (unsigned char)((utag >> 24) & 0xff);

    lua_pushboolean(tolua_S, 1);
    return 0;
failed:
    lua_pushboolean(tolua_S, 0);
    return 0;
}


int register_raw_cmd_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "RawCmd", 0);
        tolua_beginmodule(tolua_S, "RawCmd");

        // lua format: RawCmd.read_header(raw_cmd)
        // lua return: stype, ctype, utag
        tolua_function(tolua_S, "read_header", lua_raw_read_header);
        // lua format: RawCmd.set_utag(raw_cmd, utag)
        // lua return: true or false
        tolua_function(tolua_S, "set_utag", lua_raw_set_utag);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
