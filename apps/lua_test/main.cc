#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>

#include "../../netbus/proto_manager.h"
#include "../../netbus/netbus.h"
#include "../../lua_wrapper/lua_wrapper.h"


int main(int argc, char** argv) {
	netbus::instance()->init();
	lua_wrapper::init();

	if (argc < 3) {
		std::string search_path = "../../../apps/lua_test/scripts/";
		lua_wrapper::add_search_path(search_path);

		std::string lua_entry = search_path + "main.lua";
		lua_wrapper::execute_script_file(lua_entry);
	}
	else {
		std::string search_path = argv[1];
		if (search_path[search_path.length() - 1] != '/') {
			search_path += "/";
		}

		lua_wrapper::add_search_path(search_path);
		std::string lua_entry = search_path + argv[2];
		lua_wrapper::execute_script_file(lua_entry);
	}

	netbus::instance()->run();
	lua_wrapper::exit();
  return 0;
}
