#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>

#include "../utils/cache_alloc.h"
#include "session.h"
#include "session_uv.h"
#include "proto_manager.h"
#include "service_manager.h"
#include "ws_protocol.h"
#include "tcp_protocol.h"


#define SESSION_CACHE_CAPACITY 6144
#define WRITING_REQ_CACHE_CAPACITY 4096
#define WBUF_CACHE_CAPACITY 1024
#define WBUF_CACHE_SIZE 1024

struct CacheAllocator* session_allocator = NULL;
struct CacheAllocator* wrt_req_allocator = NULL;
CacheAllocator* wbuf_allocator = NULL;


void init_session_allocator()
{
	if (session_allocator == NULL) {
		session_allocator = create_cache_allocator(SESSION_CACHE_CAPACITY, sizeof(UVSession));
	}

	if (wrt_req_allocator == NULL) {
		wrt_req_allocator = create_cache_allocator(WRITING_REQ_CACHE_CAPACITY, sizeof(uv_write_t));
	}

	if (wbuf_allocator == NULL) {
		wbuf_allocator = create_cache_allocator(WBUF_CACHE_CAPACITY, WBUF_CACHE_SIZE);
	}
}


extern "C" {
	static void on_uv_close(uv_handle_t* handle) {
		UVSession* session = (UVSession*)handle->data;
		UVSession::destroy(session);
	}

	static void on_uv_shutdown(uv_shutdown_t* req, int status) {
		uv_close((uv_handle_t*)req->handle, on_uv_close);
	}

	static void after_write(uv_write_t* req, int status) {
		if (status < 0) {
			//fprintf(stderr, "Write error %s\n", uv_strerror(status));
		}
		cache_free(wrt_req_allocator, req);
	}
}

void UVSession::init()
{
	memset(&client_handler, 0, sizeof(client_handler));
	memset(client_address, 0, sizeof(client_address));
	client_port = 0;
	memset(&req_shutdown, 0, sizeof(req_shutdown));
	memset(recv_buffer, 0, sizeof(recv_buffer));
	this->is_shutdown = false;
	this->is_ws_shakehand = false;
	this->long_pkg = NULL;
	this->long_pkg_len = 0;
}

void UVSession::exit()
{
}

UVSession* UVSession::create()
{
	/*UVSession* s = (UVSession*)cache_alloc(session_allocator, sizeof(UVSession));
	s->UVSession::UVSession();*/  // this operation is unsupported in gcc
	UVSession* s = new UVSession();
	s->init();
	return s;
}

void UVSession::destroy(UVSession* s)
{
	s->exit();

	/*s->UVSession::~UVSession();
	cache_free(session_allocator, s);*/  // this operation is unsupported in gcc
	delete s;
}

void UVSession::close()
{
	if (this->is_shutdown) {
		return;
	}

	// broadcast to all service that the client is offline
	ServiceManager::on_session_disconnect(this);

	this->is_shutdown = true;
	uv_shutdown_t* req = &this->req_shutdown;
	memset(req, 0, sizeof(uv_shutdown_t));
	int ret = uv_shutdown(req, (uv_stream_t*)&this->client_handler, on_uv_shutdown);
	if (ret)  // socket not connected
	{
		uv_close((uv_handle_t*)&this->client_handler, on_uv_close);
	}
}

void UVSession::send_data(unsigned char* data, int len)
{
	uv_write_t* write_req = (uv_write_t*)cache_alloc(wrt_req_allocator, sizeof(uv_write_t));
	uv_buf_t wrbuf;

	if (this->socket_type == SESSION_TYPE_WS) {
		if (this->is_ws_shakehand) {
			int ws_len = 0;
			unsigned char* ws_data = WSProtocol::package_ws_send_data(data, len, &ws_len);

			wrbuf = uv_buf_init((char*)ws_data, ws_len);
		}
		else {
			wrbuf = uv_buf_init((char*)data, len);
		}
	}
	else if (this->socket_type == SESSION_TYPE_TCP) {
		int tcp_len = 0;
		unsigned char* tcp_data = TCPProtocol::package((const unsigned char*)data, len, &tcp_len);

		wrbuf = uv_buf_init((char*)tcp_data, tcp_len);
	}
	else {
		return;
	}

	uv_write(write_req, (uv_stream_t*)&this->client_handler, &wrbuf, 1, after_write);
}

void UVSession::send_msg(cmd_msg* msg)
{
	int encode_len = 0;
	unsigned char*  encode_data = ProtoManager::encode_msg_to_raw(msg, &encode_len);

	if (encode_data != NULL) {
		this->send_data(encode_data, encode_len);
		ProtoManager::raw_msg_free(encode_data);
	}
}

void UVSession::send_raw_cmd(raw_cmd_msg* raw)
{
	this->send_data(raw->raw_data, raw->raw_len);
}

const char* UVSession::get_address(int* client_port)
{
	*client_port = this->client_port;
	return this->client_address;
}

void* UVSession::operator new(size_t size)
{
	return cache_alloc(session_allocator, size);
}

void UVSession::operator delete(void* ptr)
{
	cache_free(session_allocator, ptr);
}


