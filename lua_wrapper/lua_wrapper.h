#ifndef __LUA_WRAPPER_H__
#define __LUA_WRAPPER_H__

#include <lua.hpp>

class lua_wrapper
{
public:
	static void init();
	static void exit();

	static bool execute_script_file(const char* file_path);

	static void register_function(const char* name, lua_CFunction func);
};

#endif // __LUA_WRAPPER_H__