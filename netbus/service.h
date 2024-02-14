#ifndef __SERVICE_H__
#define __SERVICE_H__

class Session;
struct cmd_msg;

class Service {
public:
	virtual bool on_session_recv_cmd(Session* s, cmd_msg* msg);
	virtual void on_session_disconnect(Session* s);
};


#endif