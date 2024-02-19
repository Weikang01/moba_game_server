#ifndef __SERVICE_MANAGER_H__
#define __SERVICE_MANAGER_H__

struct cmd_msg;
struct raw_cmd_msg;
class Session;
class Service;

class ServiceManager
{
public:
	static void init();
	static bool register_service(int type, Service* s);

	static void on_session_connect(Session* s);
	static bool on_recv_raw_cmd(Session* s, raw_cmd_msg* raw);
	static void on_session_disconnect(Session* s);
};




#endif // !__SERVICE_MANAGER_H__