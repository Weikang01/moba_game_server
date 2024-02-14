#ifndef __SESSION_H__
#define __SESSION_H__

struct cmd_msg;
struct raw_cmd_msg;

class Session
{
public:
	unsigned int as_client;
	unsigned int utag;
	Session()
	{
		as_client = 0;
		utag = 0;
	}
public:
	virtual void close() = 0;
	virtual void send_data(unsigned char* data, int len) = 0;
	virtual const char* get_address(int* client_port) = 0;
	virtual void send_msg(cmd_msg* msg) = 0;
	virtual void send_raw_cmd(raw_cmd_msg* raw) = 0;
};
#endif // !__SESSION_H__
