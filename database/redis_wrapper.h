#ifndef __REDIS_WRAPPER_H__
#define __REDIS_WRAPPER_H__

#include <vector>
#include <string>
#include <hiredis.h>

class redis_wrapper
{
public:
	static void connect(const char* host, int port, void (*open_cb)(const char* err, void* context));

	static void query(void* context, void(*query_cb)(const char* err, redisReply* result), const char* sql, ...);

	static void close(void* context);
};




#endif // __REDIS_WRAPPER_H__