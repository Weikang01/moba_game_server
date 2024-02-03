#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "session.h"
#include "proto_manager.h"
#include "service.h"

// return false if not valid
bool service::on_session_recv_cmd(session* s, cmd_msg* msg)
{
	return false;
}

// call when client lost connection
void service::on_session_disconnect(session* s)
{}
