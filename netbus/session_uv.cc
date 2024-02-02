#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>
#include "session.h"
#include "session_uv.h"

extern "C" {
	static void on_uv_close(uv_handle_t* handle) {
		uv_session* session = (uv_session*)handle->data;
		uv_session::destroy(session);
	}

	static void on_uv_shutdown(uv_shutdown_t* req, int status) {
		uv_close((uv_handle_t*)req->handle, on_uv_close);
	}

	static void after_write(uv_write_t* req, int status) {
		if (status < 0) {
			fprintf(stderr, "Write error %s\n", uv_strerror(status));
		}
	}
}

void uv_session::init()
{
	memset(&client_handler, 0, sizeof(client_handler));
	memset(client_address, 0, sizeof(client_address));
	client_port = 0;
	memset(&req_shutdown, 0, sizeof(req_shutdown));
	memset(&req_write, 0, sizeof(req_write));
	memset(&wrbuf, 0, sizeof(wrbuf));
	memset(recv_buffer, 0, sizeof(recv_buffer));
}

void uv_session::exit()
{
}

uv_session* uv_session::create()
{
	uv_session* s = new uv_session();
	s->init();
	return s;
}

void uv_session::destroy(uv_session* s)
{
	s->exit();
	delete s;
}

void uv_session::close()
{
	uv_shutdown_t* req = &this->req_shutdown;
	memset(req, 0, sizeof(uv_shutdown_t));
	uv_shutdown(req, (uv_stream_t*)&this->client_handler, on_uv_shutdown);
}

void uv_session::send(const char* data, int len)
{
	this->wrbuf = uv_buf_init((char*)data, len);
	memcpy(this->wrbuf.base, data, len);
	this->req_write.data = this->wrbuf.base;
	this->wrbuf.len = len;

	uv_write(&this->req_write, (uv_stream_t*)&this->client_handler, &this->wrbuf, 1, after_write);
}

const char* uv_session::get_address(int* client_port)
{
	*client_port = this->client_port;
	return this->client_address;
}
