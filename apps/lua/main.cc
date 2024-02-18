#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>

#include "../../netbus/proto_manager.h"
#include "../../netbus/netbus.h"
#include "../../lua_wrapper/lua_wrapper.h"

#undef main
int main(int argc, char** argv) {
	Netbus::instance()->init();
	LuaWrapper::init();

	if (argc < 3) {
		std::string search_path = "../../../apps/lua/scripts/";
		LuaWrapper::add_search_path(search_path);

		std::string lua_entry = search_path + "auth/main.lua";
		LuaWrapper::execute_script_file(lua_entry);
	}
	else {
		std::string search_path = argv[1];
		if (search_path[search_path.length() - 1] != '/') {
			search_path += "/";
		}

		LuaWrapper::add_search_path(search_path);
		std::string lua_entry = search_path + argv[2];
		LuaWrapper::execute_script_file(lua_entry);
	}

	Netbus::instance()->run();
	LuaWrapper::exit();
  return 0;
}
