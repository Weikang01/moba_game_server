#ifndef __SESSION_H__
#define __SESSION_H__

class session
{
public:
	virtual void close() = 0;
	virtual void send(const char* data, int len) = 0;
	virtual const char* get_address(int* client_port) = 0;
};
#endif // !__SESSION_H__
