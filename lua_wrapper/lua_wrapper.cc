#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <lua.hpp>
#include <tolua_fix.h>

#include "lua_wrapper.h"

#include "../utils/logger.h"
#include "mysql_export_to_lua.h"
#include "redis_export_to_lua.h"
#include "service_export_to_lua.h"
#include "session_export_to_lua.h"
#include "scheduler_export_to_lua.h"
#include "netbus_export_to_lua.h"
#include "proto_manager_export_to_lua.h"


lua_State* g_lua_state = NULL;

static void print_error(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_ERROR, msg);
}

static void print_warn(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_WARN, msg);
}

static void print_debug(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_DEBUG, msg);
}

static void do_log_message(void(*log)(const char* file_name, int line_num, const char* msg), const char* msg) {
	lua_Debug info;
	int depth = 0;
	while (lua_getstack(g_lua_state, depth, &info)) {
		lua_getinfo(g_lua_state, "Sln", &info);

		if (info.source[0] == '@') {
			log(&info.source[1], info.currentline, msg);
			return;
		}

		++depth;
	}
	if (depth == 0) {
		log("trunk", 0, msg);
	}
}

int lua_log_debug(lua_State* luastate) {
	int nargs = lua_gettop(luastate);
	std::string t;

	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char* str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}

	do_log_message(print_debug, t.c_str());
	return 0;
}

int lua_log_warn(lua_State* luastate) {
	int nargs = lua_gettop(luastate);
	std::string t;
	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char* str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}
	do_log_message(print_warn, t.c_str());
	return 0;
}

int lua_log_error(lua_State* luastate) {
	int nargs = lua_gettop(luastate);
	std::string t;
	for (int i = 1; i <= nargs; i++)
	{
		if (lua_istable(luastate, i))
			t += "table";
		else if (lua_isnone(luastate, i))
			t += "none";
		else if (lua_isnil(luastate, i))
			t += "nil";
		else if (lua_isboolean(luastate, i))
		{
			if (lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if (lua_isfunction(luastate, i))
			t += "function";
		else if (lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if (lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char* str = lua_tostring(luastate, i);
			if (str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if (i != nargs)
			t += "\t";
	}
	do_log_message(print_error, t.c_str());
	return 0;
}

static int lua_logger_init(lua_State* tolua_S) {
	const char* path = lua_tostring(tolua_S, 1);
	const char* prefix;
	bool std_out;
	int time_zone;
	if (path == NULL) {
		goto lua_failed;
	}

	prefix = lua_tostring(tolua_S, 2);
	if (prefix == NULL) {
		goto lua_failed;
	}

	std_out = lua_toboolean(tolua_S, 3);
	time_zone = lua_tointeger(tolua_S, 4);

	Logger::init(path, prefix, std_out, time_zone);

lua_failed:
	return 0;
}

static int register_logger_export(lua_State* tolua_S) {
	LuaWrapper::register_function("print", lua_log_debug);

	lua_getglobal(tolua_S, "_G");
	if (lua_istable(tolua_S, -1)) {
		tolua_open(tolua_S);
		tolua_module(tolua_S, "Logger", 0);
		tolua_beginmodule(tolua_S, "Logger");

		// lua format: Logger.debug("message")
		// lua return: void
		tolua_function(tolua_S, "debug", lua_log_debug);
		// lua format: Logger.warn("message")
		// lua return: void
		tolua_function(tolua_S, "warn", lua_log_warn);
		// lua format: Logger.error("message")
		// lua return: void
		tolua_function(tolua_S, "error", lua_log_error);
		// lua format: Logger.init("path", "prefix", true, 8)
		// lua return: void
		tolua_function(tolua_S, "init", lua_logger_init);
		tolua_endmodule(tolua_S);
	}
	lua_pop(tolua_S, 1);
	return 0;
}

static int lua_panic(lua_State* L) {
	const char* err = lua_tostring(L, -1);
	if (err) {
		do_log_message(print_error, err);
	}
	return 0;
}

void LuaWrapper::register_function(const char* name, lua_CFunction func)
{
	lua_pushcfunction(g_lua_state, func);
	lua_setglobal(g_lua_state, name);
}

static int lua_add_search_path(lua_State* L) {
	const char* path = luaL_checkstring(L, 1);
	if (path) {
		LuaWrapper::add_search_path(path);
	}
	return 0;
}

void LuaWrapper::init()
{
	g_lua_state = luaL_newstate();
	lua_atpanic(g_lua_state, lua_panic);
	luaL_openlibs(g_lua_state);
	toluafix_open(g_lua_state);
	LuaWrapper::register_function("add_search_path", lua_add_search_path);

	register_logger_export(g_lua_state);
	register_mysql_export(g_lua_state);
	register_redis_export(g_lua_state);
	register_service_export(g_lua_state);
	register_session_export(g_lua_state);
	register_scheduler_export(g_lua_state);
	register_netbus_export(g_lua_state);
	register_proto_manager_export(g_lua_state);
	register_raw_cmd_export(g_lua_state);
}

void LuaWrapper::exit()
{
	if (g_lua_state)
	{
		lua_close(g_lua_state);
		g_lua_state = NULL;
	}
}

bool LuaWrapper::execute_script_file(const std::string& file_path)
{
	if (luaL_dofile(g_lua_state, file_path.c_str()))
	{
		const char* error_string = lua_tostring(g_lua_state, -1);
		log_error("execute_script_file error: %s", error_string);
		lua_pop(g_lua_state, 1);
		return false;
	}
	return true;
}

lua_State* LuaWrapper::get_lua_state()
{
	return g_lua_state;
}

static bool pushFunctionByHandler(int nHandler)
{
	toluafix_get_function_by_refid(g_lua_state, nHandler);
	if (!lua_isfunction(g_lua_state, -1))
	{
		log_error("function refid '%d' does not reference a Lua function", nHandler);
		lua_pop(g_lua_state, 1);
		return false;
	}
	return true;
}

static int executeFunction(int numArgs)
{
	int functionIndex = -(numArgs + 1);
	if (!lua_isfunction(g_lua_state, functionIndex))
	{
		log_error("value at stack [%d] is not function", functionIndex);
		lua_pop(g_lua_state, numArgs + 1); // remove function and arguments
		return 0;
	}

	int traceback = 0;
	lua_getglobal(g_lua_state, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
	if (!lua_isfunction(g_lua_state, -1))
	{
		lua_pop(g_lua_state, 1);                                            /* L: ... func arg1 arg2 ... */
	}
	else
	{
		lua_insert(g_lua_state, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
		traceback = functionIndex - 1;
	}

	int error = 0;
	error = lua_pcall(g_lua_state, numArgs, 1, traceback);                  /* L: ... [G] ret */
	if (error)
	{
		if (traceback == 0)
		{
			log_error("[LUA ERROR] %s", lua_tostring(g_lua_state, -1));        /* L: ... error */
			lua_pop(g_lua_state, 1); // remove error message from stack
		}
		else                                                            /* L: ... G error */
		{
			lua_pop(g_lua_state, 2); // remove __G__TRACKBACK__ and error message from stack
		}
		return 0;
	}

	// get return value
	int ret = 0;
	if (lua_isnumber(g_lua_state, -1))
	{
		ret = (int)lua_tointeger(g_lua_state, -1);
	}
	else if (lua_isboolean(g_lua_state, -1))
	{
		ret = (int)lua_toboolean(g_lua_state, -1);
	}
	// remove return value from stack
	lua_pop(g_lua_state, 1);                                                /* L: ... [G] */

	if (traceback)
	{
		lua_pop(g_lua_state, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
	}

	return ret;
}

int LuaWrapper::execute_function_by_handler(int handler, int nargs)
{
	int ret = 0;
	if (pushFunctionByHandler(handler))
	{
		if (nargs > 0)
		{
			lua_insert(g_lua_state, -nargs - 1);
		}
		ret = executeFunction(nargs);
	}
	lua_settop(g_lua_state, 0);
	return ret;
}

void LuaWrapper::remove_script_handler(int handler)
{
	toluafix_remove_function_by_refid(g_lua_state, handler);
}

void LuaWrapper::add_search_path(const std::string& path)
{
	char script[1024] = { 0 };
	sprintf(script, "local path = string.match([[%s]],[[(.*)/[^/]*$]])\n package.path = package.path .. [[;]] .. path .. [[/?.lua;]] .. path .. [[/?/init.lua]]\n", path.c_str());
	luaL_dostring(g_lua_state, script);
}
