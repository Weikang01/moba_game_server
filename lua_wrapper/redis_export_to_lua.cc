#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lua_wrapper/lua_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <tolua++.h>
#ifdef __cplusplus
}
#endif

#include <tolua_fix.h>

#include "redis_export_to_lua.h"
#include "../database/redis_wrapper.h"

static void on_connect_cb(const char* erro, void* context, void* udata) {
    if (erro) {
		lua_pushstring(lua_wrapper::get_lua_state(), erro);
		lua_pushnil(lua_wrapper::get_lua_state());
	} 
    else {
		lua_pushnil(lua_wrapper::get_lua_state());
		tolua_pushuserdata(lua_wrapper::get_lua_state(), context);
	}
	lua_wrapper::execute_function_by_handler((int)udata, 2);
	lua_wrapper::remove_script_handler((int)udata);
}

static int lua_redis_connect(lua_State* tolua_S)
{
	const char* host;
	int port, handler;
	host = tolua_tostring(tolua_S, 1, 0);
    if (host == NULL)
        return 0;
	port = tolua_tonumber(tolua_S, 2, 0);
    if (port == NULL)
        return 0;
	handler = toluafix_ref_function(tolua_S, 3, 0);
	redis_wrapper::connect(host, port, on_connect_cb, (void*)handler);
    return 0;
}

static void push_result_to_lua(redisReply* result) {
    switch (result->type) {
	case REDIS_REPLY_INTEGER:
		lua_pushinteger(lua_wrapper::get_lua_state(), result->integer);
		break;
	case REDIS_REPLY_STATUS:
	case REDIS_REPLY_STRING:
		lua_pushstring(lua_wrapper::get_lua_state(), result->str);
		break;
	case REDIS_REPLY_ARRAY:
		lua_newtable(lua_wrapper::get_lua_state());
        for (size_t i = 0; i < result->elements; i++) {
			push_result_to_lua(result->element[i]);
			lua_rawseti(lua_wrapper::get_lua_state(), -2, i + 1);
		}
		break;
	case REDIS_REPLY_NIL:
		lua_pushnil(lua_wrapper::get_lua_state());
		break;
	case REDIS_REPLY_ERROR:
		lua_pushstring(lua_wrapper::get_lua_state(), result->str);
		break;
	default:
		lua_pushnil(lua_wrapper::get_lua_state());
		break;
	}
}

static void on_query_cb(const char* erro, redisReply* result, void* udata) {
    if (erro) {
		lua_pushstring(lua_wrapper::get_lua_state(), erro);
		lua_pushnil(lua_wrapper::get_lua_state());
	} 
    else {
		lua_pushnil(lua_wrapper::get_lua_state());
        if (result) {
            push_result_to_lua(result);
        }
        else
			lua_pushnil(lua_wrapper::get_lua_state());
	}
	lua_wrapper::execute_function_by_handler((int)udata, 2);
	lua_wrapper::remove_script_handler((int)udata);
}

static int lua_redis_query(lua_State* tolua_S)
{
    void* context = tolua_touserdata(tolua_S, 1, 0);
    if (context == NULL)
        return 0;
    const char* sql = tolua_tostring(tolua_S, 2, 0);
    if (sql == NULL)
		return 0;
    int handler = toluafix_ref_function(tolua_S, 3, 0);
    if (handler == NULL)
		return 0;

    redis_wrapper::query(context, sql, on_query_cb, (void*)handler);

    return 0;
}

static int lua_redis_close(lua_State* tolua_S)
{
    void* context = tolua_touserdata(tolua_S, 1, 0);
    if (context)
        redis_wrapper::close(context);
    return 0;
}

int register_redis_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "redis_wrapper", 0);
        tolua_beginmodule(tolua_S, "redis_wrapper");

        tolua_function(tolua_S, "connect", lua_redis_connect);
        tolua_function(tolua_S, "query", lua_redis_query);
        tolua_function(tolua_S, "close", lua_redis_close);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
