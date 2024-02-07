#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <uv.h>
#include <hiredis.h>

#include <adapters/libuv.h>

#include "redis_wrapper.h"

#define my_malloc malloc
#define my_free free


struct connect_req {
	char* host;
	int port;
	void(*open_cb)(const char* err, void* context);
	char* err;
	void* redis_context;
};

struct redis_context {
	void* context;
	int is_closing;
};

void connectCallback(const redisAsyncContext* c, int status) {

	struct connect_req* connect_req = (struct connect_req*)(c->data);

	if (status != REDIS_OK) {
		connect_req->err = strdup(c->errstr);
		connect_req->open_cb(connect_req->err, NULL);
		return;
	}
	
	struct redis_context* redis_context = (struct redis_context*)my_malloc(sizeof(struct redis_context));
	memset(redis_context, 0, sizeof(struct redis_context));

	redis_context->context = (void*)c;
	redis_context->is_closing = 0;

	connect_req->redis_context = (void*)redis_context;

	connect_req->open_cb(NULL, (void*)redis_context);
}

void redis_wrapper::connect(const char* host, const int port, void(*open_cb)(const char* err, void* context))
{
	redisAsyncContext* c = redisAsyncConnect(host, port);

	if (c->err) {
		/* Let *c leak for now... */
		printf("Error: %s\n", c->errstr);
		return;
	}

	struct connect_req* connect_req = (struct connect_req*)my_malloc(sizeof(struct connect_req));
	memset(connect_req, 0, sizeof(struct connect_req));

	connect_req->host = strdup(host);
	connect_req->port = port;
	connect_req->open_cb = open_cb;
	connect_req->err = NULL;
	connect_req->redis_context = NULL;

	c->data = (void*)connect_req;

	redisLibuvAttach(c, uv_default_loop());
	redisAsyncSetConnectCallback(c, connectCallback);
}

struct query_req {
	void* redis_context;
	char* sql;
	void(*query_cb)(const char* err, redisReply* result);
	redisReply* result;
	int type;
	char* err;
};

void getCallback(redisAsyncContext* c, void* r, void* privdata) {
	redisReply* reply = (redisReply*)r;

	struct query_req* query_req = (struct query_req*)c->data;

	if (reply == NULL) {
		query_req->err = strdup(c->errstr);
		query_req->query_cb(query_req->err, NULL);
		return;
	}

	query_req->result = reply;
	query_req->query_cb(NULL, query_req->result);
}

void redis_wrapper::query(void* context, void(*query_cb)(const char* err, redisReply* result), const char* sql, ...)
{
	struct redis_context* redis_con = (struct redis_context*)context;
	if (redis_con->is_closing) {
		return;
	}

	redisAsyncContext* c = (redisAsyncContext*)redis_con->context;

	struct query_req* query_req = (struct query_req*)my_malloc(sizeof(struct query_req));
	memset(query_req, 0, sizeof(struct query_req));

	query_req->redis_context = context;

	static char sql_content[1024] = { 0 };
	
	va_list args;
	va_start(args, sql);
	vsnprintf(sql_content, sizeof(sql_content), sql, args);
	va_end(args);


	query_req->query_cb = query_cb;
	query_req->result = NULL;
	query_req->err = NULL;

	c->data = (void*)query_req;

	redisAsyncCommand(c, getCallback, NULL, sql_content);
}

void redis_wrapper::close(void* context)
{
	struct redis_context* redis_con = (struct redis_context*)context;
	if (redis_con->is_closing) {
		return;
	}
	redisAsyncContext* c = (redisAsyncContext*)redis_con->context;
	redis_con->is_closing = 1;
	redisAsyncDisconnect(c);
}
