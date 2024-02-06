#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.hpp>

#include "../utils/logger.h"
#include "lua_wrapper.h"


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

void lua_wrapper::register_function(const char* name, lua_CFunction func)
{
	lua_pushcfunction(g_lua_state, func);
	lua_setglobal(g_lua_state, name);
}
