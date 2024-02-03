#ifndef __TCP_PROTOCOL_H__
#define __TCP_PROTOCOL_H__

class tcp_protocol
{
public:
	static bool read_header(const unsigned char* data, int data_len, int* header_len, int* package_len);
	static unsigned char* package(const unsigned char* data, int data_len, int* package_len);
	static void free_package(unsigned char* package);
};


#endif // !__TCP_PROTOCOL_H__
