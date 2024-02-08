#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <map>

#include "proto_manager.h"

#define CMD_HEADER_SIZE 8
#define CMD_TRAILER_SIZE 1

static int g_proto_type = PROTO_BUF;
static std::map<int, std::string> g_cmd_map;

void proto_manager::init(int proto_type)
{
	g_proto_type = proto_type;
}

void proto_manager::register_protobuf_cmd_map(std::map<int, std::string>& map)
{
	std::map<int, std::string>::iterator it;
	for (it = map.begin(); it != map.end(); it++) {
		g_cmd_map[it->first] = it->second;
	}
}

int proto_manager::proto_type()
{
	return g_proto_type;
}

const char* proto_manager::pb_type2name(int ctype)
{
	std::map<int, std::string>::iterator it = g_cmd_map.find(ctype);
	if (it != g_cmd_map.end()) {
		return it->second.c_str();
	}
	return NULL;
}



google::protobuf::Message* proto_manager::create_message(const std::string& typeName) {
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor) {
		const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return message;
}

void proto_manager::free_protobuf_message(google::protobuf::Message* message) {
	if (message) {
		delete message;
	}
}

// stype - 2 bytes
// ctype - 2 bytes
// utag - 4 bytes
bool proto_manager::decode_cmd_msg(const char* in_data, int in_len, cmd_msg** out_msg)
{
	if (in_len < 8) {
		return false;
	}

	struct cmd_msg* msg = (struct cmd_msg*)malloc(sizeof(struct cmd_msg));
	memset(msg, 0, sizeof(struct cmd_msg));

	msg->stype = in_data[0] | (in_data[1] << 8);
	msg->ctype = in_data[2] | (in_data[3] << 8);
	msg->utag = in_data[4] | (in_data[5] << 8) | (in_data[6] << 16) | (in_data[7] << 24);
	msg->body = NULL;

	*out_msg = msg;

	if (in_len == CMD_HEADER_SIZE) {
		return true;
	}

	if (g_proto_type == PROTO_JSON) {
		if (in_data[in_len - 1] != 0) {
			free(msg);
			*out_msg = NULL;
			return false;
		}
		msg->body = strdup(in_data + CMD_HEADER_SIZE);
		return true;
	}
	else if (g_proto_type == PROTO_BUF) {
		google::protobuf::Message* message = create_message(g_cmd_map[msg->ctype].c_str());

		if (message) {
			if (message->ParseFromArray(in_data + CMD_HEADER_SIZE, in_len - CMD_HEADER_SIZE)) {
				msg->body = message;
			}
			else {
				free_protobuf_message(message);
				free(msg);
				*out_msg = NULL;
				return false;
			}
		}
		else {
			free(msg);
			*out_msg = NULL;
			return false;
		}
	}

	return true;
}

unsigned char* proto_manager::encode_msg_to_raw(cmd_msg* msg, int* out_len)
{
	int len = CMD_HEADER_SIZE;
	if (msg->body) {
		if (g_proto_type == PROTO_JSON) {
			len += strlen((char*)msg->body);
			len += CMD_TRAILER_SIZE;
		}
		else if (g_proto_type == PROTO_BUF) {
			len += ((google::protobuf::Message*)msg->body)->ByteSize();
		}
	}

	unsigned char* out_data = (unsigned char*)malloc(len);
	out_data[0] = msg->stype & 0xff;
	out_data[1] = (msg->stype >> 8) & 0xff;
	out_data[2] = msg->ctype & 0xff;
	out_data[3] = (msg->ctype >> 8) & 0xff;
	memcpy(out_data + 4, &msg->utag, 4);
	if (msg->body) {
		if (g_proto_type == PROTO_JSON) {
			memcpy(out_data + CMD_HEADER_SIZE, msg->body, len - CMD_HEADER_SIZE - CMD_TRAILER_SIZE);
			out_data[len - 1] = 0;
		}
		else if (g_proto_type == PROTO_BUF) {
			if (!((google::protobuf::Message*)msg->body)->SerializeToArray(out_data + CMD_HEADER_SIZE, len - CMD_HEADER_SIZE)) {
				free(out_data);
				*out_len = 0;
				return NULL;
			}

			// debug: print protobuf message
			//printf("encode_cmd_msg: %s\n", ((google::protobuf::Message*)msg->body)->DebugString().c_str());
		}
	}

	*out_len = len;
	return out_data;
}

void proto_manager::cmd_msg_free(cmd_msg* msg)
{
	if (msg->body) {
		if (g_proto_type == PROTO_JSON) {
			free(msg->body);
		}
		else if (g_proto_type == PROTO_BUF) {
			free_protobuf_message((google::protobuf::Message*)msg->body);
		}
	}

	free(msg);
}

void proto_manager::raw_msg_free(char* raw_msg)
{
	free(raw_msg);
}
