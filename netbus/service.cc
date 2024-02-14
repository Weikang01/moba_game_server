#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "session.h"
#include "proto_manager.h"
#include "service.h"

Service::Service()
{
	this->use_raw_cmd = false;
}

bool Service::on_session_recv_raw(Session* s, raw_cmd_msg* raw)
{
	return false;
}

// return false if not valid
bool Service::on_session_recv_cmd(Session* s, cmd_msg* msg)
{
	return false;
}

// call when client lost connection
void Service::on_session_disconnect(Session* s)
{}
