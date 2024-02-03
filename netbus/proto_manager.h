#ifndef  __PROTO_MANAGER_H__
#define __PROTO_MANAGER_H__

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
	static void reg_cmd_map(const char** cmd_map, int cmd_count);
	static int proto_type();

	static bool decode_cmd_msg(const char* in_data, int in_len, cmd_msg** out_msg);
	static unsigned char* encode_cmd_msg(cmd_msg* msg, int* out_len);

	static void cmd_msg_free(cmd_msg* msg);
	static void raw_msg_free(char* raw_msg);
};
#endif // ! __PROTO_MANAGER_H__
