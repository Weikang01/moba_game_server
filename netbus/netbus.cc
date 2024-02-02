#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "netbus.h"

#include "session_uv.h"


static netbus* _instance = nullptr;
netbus* netbus::instance()
{
	return _instance;
}


extern "C" {
	static void on_uv_close(uv_handle_t* handle) {
		uv_session* session = (uv_session*)handle->data;
		uv_session::destroy(session);
	}

	static void on_uv_shutdown(uv_shutdown_t* req, int status) {
		uv_close((uv_handle_t*)req->handle, on_uv_close);
	}

	static void on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
		uv_session* session = (uv_session*)handle->data;
		*buf = uv_buf_init(session->recv_buffer + session->recved, SESSION_RECV_LEN - session->recved);

	}

	static void after_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
		if (nread < 0) {
			if (nread != UV_EOF) {
				fprintf(stderr, "Read error %s\n", uv_strerror(nread));
			}
			uv_session* session = (uv_session*)client->data;
			
			uv_shutdown_t* req = &session->req_shutdown;
			memset(req, 0, sizeof(uv_shutdown_t));
			uv_shutdown(req, client, on_uv_shutdown);
		}
		else {
			uv_session* session = (uv_session*)client->data;
			session->send(buf->base, nread);
			//session->close();
		}
	}

	static void on_new_connection(uv_stream_t* server, int status) {
		if (status < 0) {
			fprintf(stderr, "New connection error %s\n", uv_strerror(status));
			return;
		}

		uv_session* session = uv_session::create();

		uv_tcp_t* client = &session->client_handler;

		struct sockaddr_in addr;
		int len = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)client, (struct sockaddr*)&addr, &len);
		uv_ip4_name(&addr, session->client_address, 32);
		session->client_port = ntohs(addr.sin_port);
		session->socket_type = (int)server->data;

		printf("New connection from %s:%d\n", session->client_address, session->client_port);

		uv_tcp_init(uv_default_loop(), client);
		client->data = (void*)session;
		if (uv_accept(server, (uv_stream_t*)client) == 0) {
			uv_read_start((uv_stream_t*)client, on_alloc_buffer, after_read);
		}
		else {
			session->close();
		}
	}
}


void netbus::start_tcp_server(int port) {
	uv_tcp_t* tcp_server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(tcp_server, 0, sizeof(uv_tcp_t));

	uv_tcp_init(uv_default_loop(), tcp_server);

	struct sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", port, &addr);

	int ret = uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);

	if (ret) {
		printf("Bind error %s\n", uv_strerror(ret));
		return;
	}
	
	tcp_server->data = (void*)SESSION_TYPE_TCP;
	// Start listening
	uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_new_connection);
}

void netbus::start_ws_server(int port)
{
}

void netbus::run()
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
