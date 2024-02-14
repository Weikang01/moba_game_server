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

#include "netbus_export_to_lua.h"
#include "../netbus/netbus.h"

static int lua_tcp_listen(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    int port;
    if (argc != 1)
		goto failed;
	port = lua_tointeger(tolua_S, 1);
	if (port == NULL)
		goto failed;
	Netbus::instance()->tcp_listen(port);
failed:
    return 0;
}

static int lua_ws_listen(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    int port;
    if (argc != 1)
        goto failed;
    port = lua_tointeger(tolua_S, 1);
    if (port == NULL)
        goto failed;
    Netbus::instance()->ws_listen(port);
failed:
    return 0;
}

static int lua_udp_listen(lua_State* tolua_S)
{
    int argc = lua_gettop(tolua_S);
    int port;
    if (argc != 1)
        goto failed;
    port = lua_tointeger(tolua_S, 1);
    if (port == NULL)
        goto failed;
    Netbus::instance()->udp_listen(port);
failed:
    return 0;
}

static void on_tcp_connect(int err, Session* session, void* udata)
{
    // return (err, session)
    if (err) {
        lua_pushinteger(LuaWrapper::get_lua_state(), err);
		lua_pushnil(LuaWrapper::get_lua_state());
	}
	else {
        lua_pushinteger(LuaWrapper::get_lua_state(), err);
        tolua_pushuserdata(LuaWrapper::get_lua_state(), session);
    }
    LuaWrapper::execute_function_by_handler((int)udata, 2);
    LuaWrapper::remove_script_handler((int)udata);
}


// ip, port, callback
static int lua_tcp_connect(lua_State* tolua_S)
{
	int argc = lua_gettop(tolua_S);
	const char* ip;
	int port;
    int handler;
	if (argc != 3)
		goto failed;

	ip = luaL_checkstring(tolua_S, 1);
    if (ip == NULL)
		goto failed;
    port = lua_tointeger(tolua_S, 2);
    if (port == NULL)
        goto failed;
    handler = toluafix_ref_function(tolua_S, 3, 0);
    if (handler == 0)
		goto failed;

    Netbus::instance()->tcp_connect(ip, port, on_tcp_connect, (void*)handler);
failed:
    return 0;
}


int register_netbus_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "Netbus", 0);
        tolua_beginmodule(tolua_S, "Netbus");

        tolua_function(tolua_S, "tcp_listen", lua_tcp_listen);
        tolua_function(tolua_S, "ws_listen", lua_ws_listen);
        tolua_function(tolua_S, "udp_listen", lua_udp_listen);
        tolua_function(tolua_S, "tcp_connect", lua_tcp_connect);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
