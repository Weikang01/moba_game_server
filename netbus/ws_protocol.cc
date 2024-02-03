#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

#include <uv.h>
#include <http_parser.h>

#include "session.h"
#include "session_uv.h"
#include "ws_protocol.h"

static char field_sec_key[256];
static char value_sec_key[256];
static int is_sec_key = 0;
static int has_sec_key = 0;
static int is_shake_ended = 0;

extern "C" {
	#include <base64_encoder.h>
	#include <sha1.h>

	static int on_header_field(http_parser* parser, const char* at, size_t length)
	{
		if (strncmp("Sec-WebSocket-Key", at, length) == 0) {
			is_sec_key = 1;
			has_sec_key = 1;
			strncpy(field_sec_key, at, length);
		}
		else {
			is_sec_key = 0;
		}
		return 0;
	}

	static int on_header_value(http_parser* parser, const char* at, size_t length)
	{
		if (is_sec_key) {
			strncpy(value_sec_key, at, length);
		}
		return 0;
	}

	static int on_message_complete(http_parser* parser)
	{
		is_shake_ended = 1;
		return 0;
	}

	static char* ws_accept_key(const char* key) {
		const char* guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
		char* concat = (char*)malloc(strlen(key) + strlen(guid) + 1);
		strcpy(concat, key);
		strcat(concat, guid);
		unsigned char sha1[SHA1_DIGEST_SIZE];
		int sha1_size = 0;

		crypt_sha1((uint8_t*)concat, strlen(concat), (uint8_t*)sha1, &sha1_size);
		int base_len;
		char* base_buf = base64_encode((uint8_t*)sha1, SHA1_DIGEST_SIZE, &base_len);

		return base_buf;
	}
}

bool ws_protocol::ws_shakehand(session* s, const char* data, int len)
{
	http_parser_settings settings;
	http_parser_settings_init(&settings);

	settings.on_header_field = on_header_field;
	settings.on_header_value = on_header_value;
	settings.on_message_complete = on_message_complete;

	http_parser p;
	http_parser_init(&p, HTTP_REQUEST);
	is_sec_key = 0;
	has_sec_key = 0;
	is_shake_ended = 0;

	size_t nparsed = http_parser_execute(&p, &settings, (const char*)data, len);

	//printf("nparsed: %d\thas_sec_key: %d\tis_shake_ended: %d\n", nparsed, has_sec_key, is_shake_ended);

	if (has_sec_key && is_shake_ended) {
		static const char* response = "HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-Accept:%s\r\n"
			"WebSocket-Protocol:chat\r\n\r\n";
		// format response
		char* key = ws_accept_key(value_sec_key);
		char* response_buf = (char*)malloc(strlen(response) + strlen(key) + 1);
		sprintf(response_buf, response, key);

		s->send_data(response_buf, strlen(response_buf));

		return true;
	}
	else {
		static const char* response = "HTTP/1.1 400 Bad Request\r\n"
			"Content-Length: 0\r\n"
			"\r\n";

		s->send_data(response, strlen(response));

		return false;
	}
}

bool ws_protocol::read_ws_header(unsigned char* data, int len, int* header_len, int* payload_len)
{
	if (data[0] != 0x81 && data[0] != 0x82)
		return false;

	if (len < 6)
		return false;

	unsigned int data_len = data[1] & 0x7f;  // 0111 1111 because the first bit is the mask bit

	int head_size = 6;
	if (data_len == 0x7e) {  // 2 subsequent bytes are the length
		head_size = 8;
		if (len < 8)
			return false;
		
		data_len = (data[2] << 8) | data[3];
	}
	else if (data_len == 0x7f) {  // 8 subsequent bytes are the length
		head_size = 14;
		if (len < 14)
			return false;

		data_len = (data[2] << 56) | (data[3] << 48) | (data[4] << 40) | (data[5] << 32) |
			(data[6] << 24) | (data[7] << 16) | (data[8] << 8) | data[9];
	}

	*payload_len = data_len;
	*header_len = head_size;

	return true;
}

void ws_protocol::parse_ws_recv_data(unsigned char* data, int head_size, int payload_size)
{
	unsigned char* payload = data + head_size;
	unsigned char* mask = payload - 4;

	for (unsigned int i = 0; i < payload_size; i++) {
		payload[i] ^= mask[i % 4];
	}
}

unsigned char* ws_protocol::package_ws_send_data(const unsigned char* data, int len, int* package_len)
{
	int head_len = 2;
	if (len > 65535) {
		head_len += 8;
	}
	else if (len > 125) {
		head_len += 2;
	}

	// TODO: cache the malloc
	unsigned char* send_resp = (unsigned char*)malloc(len + head_len);
	send_resp[0] = 0x81;
	if (len < 126) {
		send_resp[1] = len;
	}
	else if (len < 65535) {
		send_resp[1] = 126;
		send_resp[2] = (len >> 8) & 0xFF;
		send_resp[3] = len & 0xFF;
	}
	else {
		send_resp[1] = 127;
		send_resp[2] = (len >> 56) & 0xFF;
		send_resp[3] = (len >> 48) & 0xFF;
		send_resp[4] = (len >> 40) & 0xFF;
		send_resp[5] = (len >> 32) & 0xFF;
		send_resp[6] = (len >> 24) & 0xFF;
		send_resp[7] = (len >> 16) & 0xFF;
		send_resp[8] = (len >> 8) & 0xFF;
		send_resp[9] = len & 0xFF;
	}

	memcpy(send_resp + head_len, data, len);

	*package_len = len + head_len;

	return send_resp;
}

void ws_protocol::free_ws_package(unsigned char* package)
{
	free(package);
}
