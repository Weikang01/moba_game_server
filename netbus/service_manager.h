#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

struct cmd_msg;
class Session;
class Service;

class ServiceManager
{
public:
	static void init();
	static bool register_service(int type, Service* s);
	static bool on_recv_cmd_msg(Session* s, cmd_msg* msg);
	static void on_session_disconnect(Session* s);
};




#endif // !__SERVICE_MANAGER_H__