#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.hpp>

#include "../utils/logger.h"
#include "lua_wrapper.h"


lua_State* g_lua_state = NULL;

void lua_wrapper::init()
{
	g_lua_state = luaL_newstate();
	luaL_openlibs(g_lua_state);
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
