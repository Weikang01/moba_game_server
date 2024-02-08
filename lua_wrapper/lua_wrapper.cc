#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.hpp>
#include <tolua_fix.h>

#include "lua_wrapper.h"

#include "../utils/logger.h"
#include "mysql_export_to_lua.h"
#include "redis_export_to_lua.h"
#include "service_export_to_lua.h"
#include "session_export_to_lua.h"


lua_State* g_lua_state = NULL;

int lua_log_debug(lua_State* L) {
	lua_Debug info;
	int depth = 0;
	while (lua_getstack(g_lua_state, depth, &info)) {
		lua_getinfo(g_lua_state, "Sln", &info);

		if (info.source[0] == '@') {
			logger::log(info.source + 1, info.currentline, LOG_DEBUG, lua_tostring(g_lua_state, -1));
		}

		depth++;
	}

	return 0;
}

int lua_log_warn(lua_State* L) {
	lua_Debug info;
	int depth = 0;
	while (lua_getstack(g_lua_state, depth, &info)) {
		lua_getinfo(g_lua_state, "Sln", &info);

		if (info.source[0] == '@') {
			logger::log(info.source + 1, info.currentline, LOG_WARN, lua_tostring(g_lua_state, -1));
		}

		depth++;
	}

	return 0;
}

int lua_log_error(lua_State* L) {
	lua_Debug info;
	int depth = 0;
	while (lua_getstack(g_lua_state, depth, &info)) {
		lua_getinfo(g_lua_state, "Sln", &info);

		if (info.source[0] == '@') {
			logger::log(info.source + 1, info.currentline, LOG_ERROR, lua_tostring(g_lua_state, -1));
		}

		depth++;
	}

	return 0;
}

static int lua_panic(lua_State* L) {
	lua_log_error(L);
	return 0;
}

void lua_wrapper::init()
{
	g_lua_state = luaL_newstate();
	lua_atpanic(g_lua_state, lua_panic);
	luaL_openlibs(g_lua_state);
	toluafix_open(g_lua_state);

	register_mysql_export(g_lua_state);
	register_redis_export(g_lua_state);
	register_service_export(g_lua_state);
	register_session_export(g_lua_state);

	// export log
	lua_register(g_lua_state, "log_debug", lua_log_debug);
	lua_register(g_lua_state, "log_warn", lua_log_warn);
	lua_register(g_lua_state, "log_error", lua_log_error);
}

void lua_wrapper::exit()
{
	if (g_lua_state)
	{
		lua_close(g_lua_state);
		g_lua_state = NULL;
	}
}

bool lua_wrapper::execute_script_file(const char* file_path)
{
	if (luaL_dofile(g_lua_state, file_path))
	{
		const char* error_string = lua_tostring(g_lua_state, -1);
		log_error("execute_script_file error: %s", error_string);
		lua_pop(g_lua_state, 1);
		return false;
	}
	return true;
}

lua_State* lua_wrapper::get_lua_state()
{
	return g_lua_state;
}

void lua_wrapper::register_function(const char* name, lua_CFunction func)
{
	lua_pushcfunction(g_lua_state, func);
	lua_setglobal(g_lua_state, name);
}

static bool pushFUnctionByHandler(int nHandler)
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

int lua_wrapper::execute_function_by_handler(int handler, int nargs)
{
	int ret = 0;
	if (pushFUnctionByHandler(handler))
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

void lua_wrapper::remove_script_handler(int handler)
{
	toluafix_remove_function_by_refid(g_lua_state, handler);
}
