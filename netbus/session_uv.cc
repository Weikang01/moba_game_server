#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>

#include "../utils/cache_alloc.h"
#include "session.h"
#include "session_uv.h"

#define SESSION_CACHE_CAPACITY 6144
#define WRITING_REQ_CACHE_CAPACITY 4096

struct cache_allocator* session_allocator = NULL;
struct cache_allocator* wrt_req_allocator = NULL;


void init_session_allocator()
{
	if (session_allocator == NULL) {
		session_allocator = create_cache_allocator(SESSION_CACHE_CAPACITY, sizeof(uv_session));
	}

	if (wrt_req_allocator == NULL) {
		wrt_req_allocator = create_cache_allocator(WRITING_REQ_CACHE_CAPACITY, sizeof(uv_write_t));
	}
}


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
		cache_free(wrt_req_allocator, req);
	}
}

void uv_session::init()
{
	memset(&client_handler, 0, sizeof(client_handler));
	memset(client_address, 0, sizeof(client_address));
	client_port = 0;
	memset(&req_shutdown, 0, sizeof(req_shutdown));
	memset(recv_buffer, 0, sizeof(recv_buffer));
	this->is_shutdown = false;
}

void uv_session::exit()
{
}

uv_session* uv_session::create()
{
	uv_session* s = (uv_session*)cache_alloc(session_allocator, sizeof(uv_session));
	s->uv_session::uv_session();
	s->init();
	return s;
}

void uv_session::destroy(uv_session* s)
{
	s->exit();

	s->uv_session::~uv_session();
	cache_free(session_allocator, s);
}

void uv_session::close()
{
	if (this->is_shutdown) {
		return;
	}

	this->is_shutdown = true;
	uv_shutdown_t* req = &this->req_shutdown;
	memset(req, 0, sizeof(uv_shutdown_t));
	uv_shutdown(req, (uv_stream_t*)&this->client_handler, on_uv_shutdown);
}

void uv_session::send(const char* data, int len)
{
	uv_write_t* write_req = (uv_write_t*)cache_alloc(wrt_req_allocator, sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init((char*)data, len);
	uv_write(write_req, (uv_stream_t*)&this->client_handler, &wrbuf, 1, after_write);
}

const char* uv_session::get_address(int* client_port)
{
	*client_port = this->client_port;
	return this->client_address;
}


