#ifndef __SESSION_H__
#define __SESSION_H__

struct cmd_msg;

class session
{
public:
	virtual void close() = 0;
	virtual void send_data(const char* data, int len) = 0;
	virtual const char* get_address(int* client_port) = 0;
	virtual void send_msg(cmd_msg* msg) = 0;
};
#endif // !__SESSION_H__
