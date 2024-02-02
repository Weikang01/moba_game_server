#ifndef __SESSION_UV_H__
#define __SESSION_UV_H__

#include "session.h"

#define SESSION_RECV_LEN 4096

enum {
	SESSION_TYPE_TCP,
	SESSION_TYPE_WS,
};

class uv_session : session
{
public:
	uv_tcp_t client_handler;
	char client_address[32];
	int client_port;

	uv_shutdown_t req_shutdown;
	uv_write_t req_write;
	uv_buf_t wrbuf;
public:
	char recv_buffer[SESSION_RECV_LEN];
	int recved;
	int socket_type;
private:
	void init();
	void exit();
	
public:
	static uv_session* create();
	static void destroy(uv_session* s);

public:
	void close() override;
	void send(const char* data, int len) override;
	const char* get_address(int* client_port) override;
};



#endif // !__SESSION_UV_H__