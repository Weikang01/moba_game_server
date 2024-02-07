#ifndef __MYSQL_WRAPPER_H__
#define __MYSQL_WRAPPER_H__

#include <vector>
#include <string>

class mysql_wrapper
{
public:
	static void connect(const char* host, int port, const char* user, const char* pass, const char* db_name, void (*open_cb)(const char* err, void* context, void* udata), void* udata=NULL);

	static void query(void* context, const char* sql, void (*query_cb)(const char* err, std::vector<std::vector<std::string>>* result));

	static void close(void* context);
};



#endif