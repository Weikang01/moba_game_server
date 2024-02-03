#ifndef __WS_PROTOCOL_H__
#define __WS_PROTOCOL_H__

class session;
class ws_protocol
{
public:
	static bool ws_shakehand(session* s, const char* data, int len);
	static bool read_ws_header(unsigned char* data, int len, int* header_len, int* payload_len);
	static void parse_ws_recv_data(unsigned char* data, int header_len, int payload_len);
	static unsigned char* package_ws_send_data(const unsigned char* data, int len, int* package_len);
	static void free_ws_package(unsigned char* package);
};


#endif // !__WS_PROTOCOL_H__
