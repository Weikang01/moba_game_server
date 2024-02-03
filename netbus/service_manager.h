#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

struct cmd_msg;
class session;
class service;

class service_manager
{
public:
	static void init();
	static bool register_service(int type, service* s);
	static bool on_recv_cmd_msg(session* s, cmd_msg* msg);
	static void on_session_disconnect(session* s);
};




#endif // !__SERVICE_MANAGER_H__