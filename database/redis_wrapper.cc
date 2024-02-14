#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>
#include <hiredis.h>

#include <adapters/libuv.h>

#include "redis_wrapper.h"
#include "../utils/small_alloc.h"

#define my_malloc small_alloc
#define my_free small_free


static char* my_strdup(const char* src) {
	char* dst = (char*)my_malloc(strlen(src) + 1);
	strcpy(dst, src);
	return dst;
}

static void free_strdup(char* str) {
	my_free(str);
}


struct connect_req {
	char* host;
	int port;
	void(*open_cb)(const char* err, void* context, void* udata);
	char* err;
	void* redis_context;
	void* udata;
};

struct redis_context {
	void* context;
	int is_closing;
};

void connectCallback(const redisAsyncContext* c, int status) {

	struct connect_req* connect_req = (struct connect_req*)(c->data);

	if (status != REDIS_OK) {
		connect_req->err = my_strdup(c->errstr);
		connect_req->open_cb(connect_req->err, NULL, connect_req->udata);
		return;
	}
	
	struct redis_context* redis_context = (struct redis_context*)my_malloc(sizeof(struct redis_context));
	memset(redis_context, 0, sizeof(struct redis_context));

	redis_context->context = (void*)c;
	redis_context->is_closing = 0;

	connect_req->redis_context = (void*)redis_context;

	connect_req->open_cb(NULL, (void*)redis_context, connect_req->udata);


	if (connect_req->host) {
		free_strdup(connect_req->host);
	}
	if (connect_req->err) {
		free_strdup(connect_req->err);
	}
	my_free(connect_req);
}

void RedisWrapper::connect(const char* host, const int port, void(*open_cb)(const char* err, void* context, void* udata), void* udata)
{
	redisAsyncContext* c = redisAsyncConnect(host, port);

	if (c->err) {
		/* Let *c leak for now... */
		printf("Error: %s\n", c->errstr);
		return;
	}

	struct connect_req* connect_req = (struct connect_req*)my_malloc(sizeof(struct connect_req));
	memset(connect_req, 0, sizeof(struct connect_req));

	connect_req->host = my_strdup(host);
	connect_req->port = port;
	connect_req->open_cb = open_cb;
	connect_req->err = NULL;
	connect_req->redis_context = NULL;
	connect_req->udata = udata;

	c->data = (void*)connect_req;

	redisLibuvAttach(c, uv_default_loop());
	redisAsyncSetConnectCallback(c, connectCallback);
}

struct query_req {
	void* redis_context;
	char* sql;
	void(*query_cb)(const char* err, redisReply* result, void* udata);
	redisReply* result;
	int type;
	char* err;
	void* udata;
};

void getCallback(redisAsyncContext* c, void* r, void* privdata) {
	redisReply* reply = (redisReply*)r;

	struct query_req* query_req = (struct query_req*)c->data;

	if (reply == NULL) {
		query_req->err = my_strdup(c->errstr);
		query_req->query_cb(query_req->err, NULL, query_req->udata);
		return;
	}

	query_req->result = reply;
	query_req->query_cb(NULL, query_req->result, query_req->udata);

	if (query_req->sql) {
		free_strdup(query_req->sql);
	}
	if (query_req->err) {
		free_strdup(query_req->err);
	}
	my_free(query_req);
	c->data = NULL;
}

void RedisWrapper::query(void* context, const char* sql, void(*query_cb)(const char* err, redisReply* result, void* udata), void* udata)
{
	struct redis_context* redis_con = (struct redis_context*)context;
	if (redis_con->is_closing) {
		return;
	}

	redisAsyncContext* c = (redisAsyncContext*)redis_con->context;

	struct query_req* query_req = (struct query_req*)my_malloc(sizeof(struct query_req));
	memset(query_req, 0, sizeof(struct query_req));

	query_req->redis_context = context;
	query_req->sql = my_strdup(sql);
	query_req->query_cb = query_cb;
	query_req->result = NULL;
	query_req->err = NULL;
	query_req->udata = udata;

	c->data = (void*)query_req;

	redisAsyncCommand(c, getCallback, NULL, sql);
}

void RedisWrapper::close(void* context)
{
	struct redis_context* redis_con = (struct redis_context*)context;
	if (redis_con->is_closing) {
		return;
	}
	redisAsyncContext* c = (redisAsyncContext*)redis_con->context;
	redis_con->is_closing = 1;
	redisAsyncDisconnect(c);
}
