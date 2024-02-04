#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>

#include "../../utils/logger.h"
#include "../../utils/time_list.h"
#include "../../utils/timestamp.h"
#include "../../netbus/proto_manager.h"
#include "../../netbus/netbus.h"
#include "proto/pf_cmd_map.h"

static void on_logger_timer(void* user_data) {
	log_debug("on_logger_timer");
}

int main(int argc, char** argv) {
	proto_manager::init(PROTO_BUF);
	pf_cmd_map_init();
	logger::init("log/test", "netbus_log", true, LOG_TIMEZONE_EST);

	schedule(on_logger_timer, NULL, 3000, 3000, -1);

	netbus::instance()->init();
	netbus::instance()->start_tcp_server(8023);
	netbus::instance()->start_ws_server(8043);

	netbus::instance()->run();
  return 0;
}
