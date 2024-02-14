#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "session.h"
#include "proto_manager.h"
#include "service.h"

// return false if not valid
bool Service::on_session_recv_cmd(Session* s, cmd_msg* msg)
{
	return false;
}

// call when client lost connection
void Service::on_session_disconnect(Session* s)
{}
