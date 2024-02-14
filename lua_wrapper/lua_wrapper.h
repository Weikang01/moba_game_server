#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

#include <string>
#include <lua.hpp>

class LuaWrapper
{
public:
	static void init();
	static void exit();

	static bool execute_script_file(const std::string& file_path);
	static lua_State* get_lua_state();

	static void register_function(const char* name, lua_CFunction func);
	static void add_search_path(const std::string& path);
public:
	static int execute_function_by_handler(int handler, int nargs);
	static void remove_script_handler(int handler);
};

#endif // __LUA_WRAPPER_H__