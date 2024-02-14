#ifndef __UDP_SESSION_H__
#define __UDP_SESSION_H__

#include "session.h"

class Session;
struct cmd_msg;

class UDPSession : public Session
{
public:
	uv_udp_t* udp_handler;
	char client_address[32];
	int client_port;
	const struct sockaddr* addr;

public:
	virtual void close() override;
	virtual void send_data(unsigned char* data, int len) override;
	virtual void send_msg(cmd_msg* msg) override;
	virtual const char* get_address(int* client_port) override;
};


#endif // !__UDP_SESSION_H__
