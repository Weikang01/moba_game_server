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

#include "mysql_export_to_lua.h"
#include "../database/mysql_wrapper.h"

static void on_connect_cb(const char* erro, void* context, void* udata) {
    if (erro) {
        lua_pushstring(LuaWrapper::get_lua_state(), erro);
        lua_pushnil(LuaWrapper::get_lua_state());
    } 
    else {
		lua_pushnil(LuaWrapper::get_lua_state());
		tolua_pushuserdata(LuaWrapper::get_lua_state(), context);
    }

    LuaWrapper::execute_function_by_handler((int)udata, 2);
    LuaWrapper::remove_script_handler((int)udata);
}

int lua_mysql_connect(lua_State* tolua_S)
{
    char* host;
    char* user;
    char* pass;
    char* db_name;
    int port, handler;
    host = (char*)tolua_tostring(tolua_S, 1, 0);
    if (host == NULL)
        goto failed;
    port = (int)tolua_tonumber(tolua_S, 2, 0);
    if (port == NULL)
        goto failed;
    user = (char*)tolua_tostring(tolua_S, 3, 0);
    if (user == NULL)
		goto failed;
    pass = (char*)tolua_tostring(tolua_S, 4, 0);
	if (pass == NULL)
        goto failed;
	db_name = (char*)tolua_tostring(tolua_S, 5, 0);
    if (db_name == NULL)
		goto failed;
    handler = toluafix_ref_function(tolua_S, 6, 0);
    MySQLWrapper::connect(host, port, user, pass, db_name, on_connect_cb, (void*)handler);

failed:
    return 0;
}

void on_query_cb(const char* erro, sql::ResultSet* result, void* udata) {
    if (erro) {
		lua_pushstring(LuaWrapper::get_lua_state(), erro);
		lua_pushnil(LuaWrapper::get_lua_state());
	} 
    else {
        lua_pushnil(LuaWrapper::get_lua_state());
        if (result) {
            lua_newtable(LuaWrapper::get_lua_state());
            int index = 1;
            int num = result->getMetaData()->getColumnCount();
            sql::ResultSetMetaData* meta = result->getMetaData();

            while (result->next()) {
				lua_newtable(LuaWrapper::get_lua_state());
                for (int i = 0; i < num; i++) {
					lua_pushstring(LuaWrapper::get_lua_state(), meta->getColumnLabel(i + 1).c_str());
					lua_pushstring(LuaWrapper::get_lua_state(), result->getString(i + 1).c_str());
					lua_settable(LuaWrapper::get_lua_state(), -3);
				}
				lua_rawseti(LuaWrapper::get_lua_state(), -2, index);
				++index;
			}
        }
        else {
            lua_pushnil(LuaWrapper::get_lua_state());
        }
	}
	LuaWrapper::execute_function_by_handler((int)udata, 2);
	LuaWrapper::remove_script_handler((int)udata);
}

int lua_mysql_query(lua_State* tolua_S)
{
    void* context = tolua_touserdata(tolua_S, 1, 0);
    if (context == NULL)
		return 0;
    char* sql = (char*)tolua_tostring(tolua_S, 2, 0);
    if (sql == NULL)
        return 0;
    int handler = toluafix_ref_function(tolua_S, 3, 0);

    MySQLWrapper::query(context, sql, on_query_cb, (void*)handler);

    return 0;
}

int lua_mysql_close(lua_State* tolua_S)
{
    void* context = tolua_touserdata(tolua_S, 1, 0);
    if (context)
        MySQLWrapper::close(context);

    return 0;
}

int register_mysql_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "MySQL", 0);
        tolua_beginmodule(tolua_S, "MySQL");

        tolua_function(tolua_S, "connect", lua_mysql_connect);
        tolua_function(tolua_S, "query", lua_mysql_query);
        tolua_function(tolua_S, "close", lua_mysql_close);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
