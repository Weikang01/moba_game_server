#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>

#include "netbus.h"
#include "session_uv.h"
#include "udp_session.h"
#include "ws_protocol.h"
#include "tcp_protocol.h"
#include "proto_manager.h"
#include "service_manager.h"

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

	struct udp_recv_buffer {
		char* receive_buffer;
		size_t max_recv_len;
	};

	static void on_udp_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
		struct udp_recv_buffer* recv_buf = (struct udp_recv_buffer*)handle->data;

		suggested_size = suggested_size < 8096 ? 8096 : suggested_size;

		if (recv_buf->max_recv_len < suggested_size) {
			if (recv_buf->receive_buffer != NULL) {
				free(recv_buf->receive_buffer);
				recv_buf->receive_buffer = NULL;
			}

			recv_buf->receive_buffer = (char*)malloc(suggested_size);
			recv_buf->max_recv_len = suggested_size;
		}

		buf->base = recv_buf->receive_buffer;
		buf->len = recv_buf->max_recv_len;
	}

	static void on_alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
		uv_session* session = (uv_session*)handle->data;

		if (session->recved < SESSION_RECV_LEN) {
			*buf = uv_buf_init(session->recv_buffer + session->recved, SESSION_RECV_LEN - session->recved);
		}
		else {
			if (session->long_pkg == NULL) { // alloc memory
				if (session->socket_type == SESSION_TYPE_WS && session->is_ws_shakehand) {
					int payload_size = 0;
					int head_size = 0;
					ws_protocol::read_ws_header((unsigned char*)session->recv_buffer, session->recved, &head_size, &payload_size);
					
					session->long_pkg = (char*)malloc(head_size + payload_size);
					memcpy(session->long_pkg, session->recv_buffer, session->recved);

					session->long_pkg_len = head_size + payload_size;
				}
				else { // tcp package > receive length
				}
			}
			*buf = uv_buf_init(session->long_pkg + session->recved, session->long_pkg_len - session->recved);
		}
	}

	static void on_recv_client_command(session* client_session, unsigned char* payload, int len) {
		// print first "len" bytes of payload (string)
		client_session->send_data(payload, len);

		struct cmd_msg* msg = NULL;
		if (proto_manager::decode_cmd_msg((const char*)payload, len, &msg)) {

			if (!service_manager::on_recv_cmd_msg((session*)client_session, msg)) 
			{
				client_session->close();
			}
			proto_manager::cmd_msg_free(msg);

		}
		else {
			//printf("decode cmd msg failed\n");
		}
	}

	static void on_receive_tcp_data(uv_session* slient_session) {
		unsigned char* package_data = (unsigned char*)((slient_session->long_pkg == NULL) ? slient_session->recv_buffer : slient_session->long_pkg);

		while (slient_session->recved > 0) {
			int payload_size = 0;
			int head_size = 0;

			if (!tcp_protocol::read_header(package_data, slient_session->recved, &head_size, &payload_size)) {
				break;
			}

			if (slient_session->recved < payload_size + head_size) {
				break;
			}

			on_recv_client_command((session*)slient_session, package_data + head_size, payload_size);

			if (slient_session->recved > payload_size + head_size) {
				memmove(package_data, package_data + payload_size + head_size, payload_size + head_size);
			}

			slient_session->recved -= payload_size + head_size;

			if (slient_session->recved == 0 && slient_session->long_pkg != NULL) {
				free(slient_session->long_pkg);
				slient_session->long_pkg = NULL;
				slient_session->long_pkg_len = 0;
			}
		}
	}

	static void on_receive_ws_data(uv_session* client_session) {
		unsigned char* package_data = (unsigned char*)((client_session->long_pkg == NULL) ? client_session->recv_buffer : client_session->long_pkg);

		while (client_session->recved > 0) {
			int payload_size = 0;
			int head_size = 0;

			if (package_data[0] == 0x88) { // client close
				client_session->close();
				break;
			}

			// package_size - head_size = body_size
			if (!ws_protocol::read_ws_header(package_data, client_session->recved, &head_size, &payload_size)) {
				break;
			}

			if (client_session->recved < payload_size + head_size) {
				break;
			}

			ws_protocol::parse_ws_recv_data(package_data, head_size, payload_size);

			on_recv_client_command((session*)client_session, package_data + head_size, payload_size);

			if (client_session->recved > payload_size + head_size) {
				memmove(package_data, package_data + payload_size + head_size, payload_size + head_size);
			}

			client_session->recved -= payload_size + head_size;

			if (client_session->recved == 0 && client_session->long_pkg != NULL) {
				free(client_session->long_pkg);
				client_session->long_pkg = NULL;
				client_session->long_pkg_len = 0;
			}
		}
	}

	static void after_read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) {
		uv_session* client_session = (uv_session*)client->data;
		if (nread < 0) {
			client_session->close();
			return;
		}

		client_session->recved += nread;
		
		if (client_session->socket_type == SESSION_TYPE_WS) {
			if (client_session->is_ws_shakehand == 0) { // shakehand
				if (ws_protocol::ws_shakehand((session*)client_session, buf->base, (int)nread)) {
					client_session->is_ws_shakehand = 1;
					client_session->recved = 0;
				}
				else {
					client_session->close();
				}
			}
			else { // receive data
				on_receive_ws_data(client_session);
			}
		}
		else if (client_session->socket_type == SESSION_TYPE_TCP) {
			on_receive_tcp_data(client_session);
		}
		else {}

	}

	static void after_udp_read(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)
	{
		udp_session udp_session;
		udp_session.udp_handler = handle;
		udp_session.addr = addr;
		uv_ip4_name((const struct sockaddr_in*)addr, udp_session.client_address, 32);

		udp_session.client_port = ntohs(((const struct sockaddr_in*)addr)->sin_port);

		on_recv_client_command((session*)&udp_session, (unsigned char*)buf->base, nread);
	}

	static void on_new_connection(uv_stream_t* server, int status) {
		if (status < 0) {
			//fprintf(stderr, "New connection error %s\n", uv_strerror(status));
			return;
		}

		uv_session* session = uv_session::create();

		uv_tcp_t* client = &session->client_handler;

		struct sockaddr_in addr;
		int len = sizeof(addr);
		uv_tcp_getpeername((uv_tcp_t*)client, (struct sockaddr*)&addr, &len);
		uv_ip4_name(&addr, session->client_address, 32);
		session->client_port = ntohs(addr.sin_port);
		session->socket_type = (int)(server->data);

		//printf("New connection from %s:%d\tsocket type:%d\n", session->client_address, session->client_port, session->socket_type);

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

void netbus::init()
{
	service_manager::init();
	init_session_allocator();
}

void netbus::start_tcp_server(int port) {
	uv_tcp_t* tcp_server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(tcp_server, 0, sizeof(uv_tcp_t));

	uv_tcp_init(uv_default_loop(), tcp_server);

	struct sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", port, &addr);

	int ret = uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);

	if (ret) {
		//printf("Bind error %s\n", uv_strerror(ret));
		return;
	}
	
	tcp_server->data = (void*)SESSION_TYPE_TCP;
	// Start listening
	uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_new_connection);
}

void netbus::start_ws_server(int port)
{
	uv_tcp_t* tcp_server = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	memset(tcp_server, 0, sizeof(uv_tcp_t));

	uv_tcp_init(uv_default_loop(), tcp_server);

	struct sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", port, &addr);

	int ret = uv_tcp_bind(tcp_server, (const struct sockaddr*)&addr, 0);

	if (ret) {
		//printf("Bind error %s\n", uv_strerror(ret));
		return;
	}

	tcp_server->data = (void*)SESSION_TYPE_WS;
	// Start listening
	uv_listen((uv_stream_t*)tcp_server, SOMAXCONN, on_new_connection);
}

void netbus::start_udp_server(int port)
{
	uv_udp_t* udp_server = (uv_udp_t*)malloc(sizeof(uv_udp_t));
	memset(udp_server, 0, sizeof(uv_udp_t));
	uv_udp_init(uv_default_loop(), udp_server);

	struct udp_recv_buffer* buffer = (struct udp_recv_buffer*)malloc(sizeof(struct udp_recv_buffer));
	memset(buffer, 0, sizeof(struct udp_recv_buffer));

	udp_server->data = (void*)buffer;

	struct sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", port, &addr);

	uv_udp_bind(udp_server, (const struct sockaddr*)&addr, 0);

	uv_udp_recv_start(udp_server, on_udp_alloc_buffer, after_udp_read);
}

void netbus::run()
{
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
