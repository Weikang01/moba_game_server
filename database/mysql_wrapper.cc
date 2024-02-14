#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <mysql/jdbc.h>
#include <uv.h>

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "mysqlcppconn.lib")

#include "mysql_wrapper.h"
#include "../utils/small_alloc.h"

#define my_malloc small_alloc
#define my_free small_free

struct connect_req {
	char* host;
	int port;
	char* user;
	char* pass;
	char* db_name;
	void(*open_cb)(const char* err, void* context, void* udata);
	void* context;
	char* err;
	void* udata;
};

struct mysql_context {
	void* context;
	uv_mutex_t lock;
	int is_closing;
};

static char* my_strdup(const char* src) {
	char* dst = (char*)my_malloc(strlen(src) + 1);
	strcpy(dst, src);
	return dst;
}

static void free_strdup(char* str) {
	my_free(str);
}


static void connect_work(uv_work_t* req) {
	struct connect_req* connect_req = (struct connect_req*)req->data;
	sql::Driver* driver = get_driver_instance();

	// concatenate host and port
	char* host = (char*)my_malloc(strlen(connect_req->host) + 10);
	sprintf(host, "%s:%d", connect_req->host, connect_req->port);

	sql::Connection* con = NULL;

	try
	{
		con = driver->connect(host, connect_req->user, connect_req->pass);

		if (!con || !con->isValid())
		{
			connect_req->context = NULL;
			connect_req->err = my_strdup("connect failed");
			return;
		}
	}
	catch (const std::exception&)
	{
		connect_req->context = NULL;
		connect_req->err = my_strdup("connect failed");
		return;
	}
	
	// Attempt to set the schema
	try
	{
		con->setSchema(connect_req->db_name);
	}
	catch (const sql::SQLException& e)
	{
		// Handle the error, e.g., by logging the exception message and setting an error flag
		connect_req->context = NULL;
		connect_req->err = my_strdup("Failed to set schema");
		// Optionally, log e.what() or e.getErrorCode() for more details
		return;
	}

	struct mysql_context* c = (struct mysql_context*)my_malloc(sizeof(struct mysql_context));
	c->context = con;
	uv_mutex_init(&c->lock);
	c->is_closing = 0;

	connect_req->context = c;
	connect_req->err = NULL;
}

static void on_connect_work_complete(uv_work_t* req, int status) {
	if (!req) return;  // Add a check to ensure req is not NULL
	struct connect_req* connect_req = (struct connect_req*)(req->data);
	if (!connect_req) return;  // Add a check to ensure connect_req is not NULL

	if (connect_req->open_cb) {
		connect_req->open_cb(connect_req->err, connect_req->context, connect_req->udata);
	}

	if (connect_req->host) {
		free_strdup(connect_req->host);
		connect_req->host = NULL;  // Nullify the pointer after freeing
	}
	if (connect_req->db_name) {
		free_strdup(connect_req->db_name);
		connect_req->db_name = NULL;
	}
	if (connect_req->user) {
		free_strdup(connect_req->user);
		connect_req->user = NULL;
	}
	if (connect_req->pass) {
		free_strdup(connect_req->pass);
		connect_req->pass = NULL;
	}
	if (connect_req->err) {
		free_strdup(connect_req->err);
		connect_req->err = NULL;
	}
	if (req->data) {
		my_free(req->data);
		req->data = NULL;  // Nullify req->data to prevent double free
	}

	my_free(req);
}

void MySQLWrapper::connect(const char* host, const int port, const char* user, const char* pass, const char* db_name, void (*open_cb)(const char* err, void* context, void* udata), void* udata)
{
	uv_work_t* req = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(req, 0, sizeof(uv_work_t));

	struct connect_req* connect_req = (struct connect_req*)my_malloc(sizeof(struct connect_req));
	memset(connect_req, 0, sizeof(struct connect_req));

	connect_req->host = my_strdup(host);
	connect_req->port = port;
	connect_req->user = my_strdup(user);
	connect_req->pass = my_strdup(pass);
	connect_req->db_name = my_strdup(db_name);
	connect_req->open_cb = open_cb;
	connect_req->err = NULL;
	connect_req->udata = udata;

	req->data = (void*)connect_req;

	uv_queue_work(uv_default_loop(), req, connect_work, on_connect_work_complete);
}

struct query_req {
	void* context;
	char* sql;
	void(*query_cb)(const char* err, sql::ResultSet* result, void* udata);
	sql::ResultSet* result;
	char* err;
	void* udata;
};

static void query_work(uv_work_t* req) {
	struct query_req* r = (struct query_req*)(req->data);
	struct mysql_context* my_conn = (struct mysql_context*)(r->context);

	uv_mutex_lock(&my_conn->lock);

	try
	{
		sql::Connection* context = (sql::Connection*)my_conn->context;
		sql::Statement* stmt = context->createStatement();
		sql::ResultSet* res = stmt->executeQuery(r->sql);

		if (res->rowsCount() > 0)
			r->result = res;
		else
			r->result = NULL;
		r->err = NULL;

		//delete res;
		delete stmt;
	}
	catch (const std::exception& e)
	{
		// if e is "no result available" then it's not an error
		if (strcmp(e.what(), "No result available") == 0)
		{
			r->result = NULL;
			r->err = NULL;
		}
		else
		{
			r->result = NULL;
			r->err = (char*)e.what();
			printf("query_work exception: %s\n", e.what());
		}
	}

	uv_mutex_unlock(&my_conn->lock);
}

static void on_query_work_complete(uv_work_t* req, int status) {
	query_req* r = (query_req*)req->data;
	r->query_cb(r->err, r->result, r->udata);

	int has_result = 0;
	
	if (r->sql)
		free_strdup(r->sql);
	if (r->result)
	{
		//delete r->result;
		has_result = 1;
		//if (r->err)
		//	free(r->err);
	}

	my_free(r);
	my_free(req);
}


void MySQLWrapper::query(void* context, const char* sql, void(*query_cb)(const char* err, sql::ResultSet* result, void* udata), void* udata)
{
	struct mysql_context* c = (struct mysql_context*)context;
	if (c->is_closing) {
		return;
	}

	uv_work_t* req = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(req, 0, sizeof(uv_work_t));

	struct query_req* r = (struct query_req*)my_malloc(sizeof(struct query_req));
	memset(r, 0, sizeof(struct query_req));

	r->context = context;
	r->sql = my_strdup(sql);
	r->query_cb = query_cb;
	r->udata = udata;

	req->data = r;
	uv_queue_work(uv_default_loop(), req, query_work, on_query_work_complete);
}

static void close_work(uv_work_t* req) {
	struct mysql_context* c = (struct mysql_context*)(req->data);

	uv_mutex_lock(&c->lock);
	sql::Connection* context = (sql::Connection*)c->context;
	context->close();
	uv_mutex_unlock(&c->lock);
}

static void on_close_work_complete(uv_work_t* req, int status) {
	struct mysql_context* c = (struct mysql_context*)(req->data);
	my_free(c);
	my_free(req);
}

void MySQLWrapper::close(void* context)
{
	struct mysql_context* c = (struct mysql_context*)context;
	if (c->is_closing)
		return;

	uv_work_t* req = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(req, 0, sizeof(uv_work_t));
	req->data = context;
	c->is_closing = 1;

	uv_queue_work(uv_default_loop(), req, close_work, on_close_work_complete);
}
