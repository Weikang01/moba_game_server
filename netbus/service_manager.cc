#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proto_manager.h"
#include "session.h"
#include "service.h"
#include "service_manager.h"

#define MAX_SERVICE 512
static Service* g_service_set[MAX_SERVICE];

void ServiceManager::init()
{
	memset(g_service_set, 0, MAX_SERVICE);
}

bool ServiceManager::register_service(int type, Service* s)
{
	if (type < 0 || type >= MAX_SERVICE) {
		return false;
	}

	if (g_service_set[type] != NULL) {
		return false;
	}

	g_service_set[type] = s;

	return true;
}

bool ServiceManager::on_recv_cmd_msg(Session* s, cmd_msg* msg)
{
	if (g_service_set[msg->stype] == NULL) {
		return false;
	}

	return g_service_set[msg->stype]->on_session_recv_cmd(s, msg);
}

void ServiceManager::on_session_disconnect(Session* s)
{
	for (int i = 0; i < MAX_SERVICE; i++) {
		if (g_service_set[i] != NULL) {
			g_service_set[i]->on_session_disconnect(s);
		}
	}
}
