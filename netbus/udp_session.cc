#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>

#include "session.h"
#include "udp_session.h"
#include "proto_manager.h"
#include "service_manager.h"

void udp_session::close()
{
}

static void on_uv_send_finished(uv_udp_send_t* req, int status)
{
	if (status) {
		//fprintf(stderr, "error on_uv_send_finished: %s\n", uv_strerror(status));
		return;
	}
	free(req);
}

void udp_session::send_data(unsigned char* data, int len)
{
	uv_udp_send_t* send_req = (uv_udp_send_t*)malloc(sizeof(uv_udp_send_t));
	uv_buf_t send_buf = uv_buf_init((char*)data, len);
	int send_ret = uv_udp_send(send_req, this->udp_handler, &send_buf, 1, this->addr, on_uv_send_finished);

}

void udp_session::send_msg(cmd_msg* msg)
{
	int encode_len = 0;
	unsigned char* encode_data = proto_manager::encode_msg_to_raw(msg, &encode_len);

	if (encode_data != NULL) {
		this->send_data(encode_data, encode_len);
		proto_manager::cmd_msg_free(msg);
	}
	else {
		printf("encode_msg_to_raw failed\n");
	}

}

const char* udp_session::get_address(int* client_port)
{
	*client_port = this->client_port;
	return this->client_address;
}
