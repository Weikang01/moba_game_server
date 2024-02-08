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
	netbus::instance()->tcp_listen(port);
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
    netbus::instance()->ws_listen(port);
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
    netbus::instance()->udp_listen(port);
failed:
    return 0;
}


int register_netbus_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "netbus", 0);
        tolua_beginmodule(tolua_S, "netbus");

        tolua_function(tolua_S, "tcp_listen", lua_tcp_listen);
        tolua_function(tolua_S, "ws_listen", lua_ws_listen);
        tolua_function(tolua_S, "udp_listen", lua_udp_listen);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
