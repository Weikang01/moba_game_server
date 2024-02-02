#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>

#include "../../netbus/netbus.h"

int main(int argc, char** argv) {
	netbus::instance()->start_tcp_server(8023);
	netbus::instance()->start_tcp_server(8024);

	netbus::instance()->run();
  return 0;
}
