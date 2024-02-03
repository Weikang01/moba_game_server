#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>

#include "../../netbus/proto_manager.h"
#include "../../netbus/netbus.h"
#include "proto/pf_cmd_map.h"

int main(int argc, char** argv) {
	proto_manager::init(PROTO_BUF);
	pf_cmd_map_init();

	netbus::instance()->init();
	netbus::instance()->start_tcp_server(8023);
	netbus::instance()->start_ws_server(8043);

	netbus::instance()->run();
  return 0;
}
