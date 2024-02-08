#ifndef  __PROTO_MANAGER_H__
#define __PROTO_MANAGER_H__
#include <string>
#include <map>
#include <google/protobuf/message.h>

enum {
	PROTO_JSON = 0,
	PROTO_BUF = 1,
};

struct cmd_msg {
	int stype; // service type
	int ctype; // cmd type
	int utag; // user tag
	void* body; // json string or binary buffer
};

class proto_manager {
public:
	static void init(int proto_type);
	static void reg_cmd_map(std::map<int, std::string>& map);
	static int proto_type();
	static const char* pb_type2name(int ctype);

	static google::protobuf::Message* create_message(const std::string& typeName);
	static void free_protobuf_message(google::protobuf::Message* message);

	static bool decode_cmd_msg(const char* in_data, int in_len, cmd_msg** out_msg);
	static unsigned char* encode_msg_to_raw(cmd_msg* msg, int* out_len);

	static void cmd_msg_free(cmd_msg* msg);
	static void raw_msg_free(char* raw_msg);
};
#endif // ! __PROTO_MANAGER_H__
