#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <lua.hpp>
#include <tolua_fix.h>

#include "logger_export_to_lua.h"
#include "lua_wrapper.h"

#include "../utils/logger.h"


void print_error(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_ERROR, msg);
}

void print_warn(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_WARN, msg);
}

void print_debug(const char* file_name, int line_num, const char* msg) {
	Logger::log(file_name, line_num, LOG_DEBUG, msg);
}

void do_log_message(void(*log)(const char* file_name, int line_num, const char* msg), const char* msg) {
	lua_Debug info;
	int depth = 0;
	while (lua_getstack(LuaWrapper::get_lua_state(), depth, &info)) {
		lua_getinfo(LuaWrapper::get_lua_state(), "Sln", &info);

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

int register_logger_export(lua_State* tolua_S) {
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