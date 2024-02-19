#ifndef __SERVICE_H__
#define __SERVICE_H__

class Session;
struct raw_cmd_msg;
struct cmd_msg;

class Service {
public:
	bool use_raw_cmd;
	Service();
public:
	virtual void on_session_connect(Session* s, int stype);
	virtual bool on_session_recv_raw(Session* s, raw_cmd_msg* raw);
	virtual bool on_session_recv_cmd(Session* s, cmd_msg* msg);
	virtual void on_session_disconnect(Session* s, int stype);
};
#endif