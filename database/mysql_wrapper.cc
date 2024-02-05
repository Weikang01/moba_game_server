#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <mysql/jdbc.h>
#include <uv.h>

#pragma comment(lib, "libcrypto.lib")
#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "mysqlcppconn.lib")

#include "mysql_wrapper.h"

#define my_malloc malloc
#define my_free free

struct connect_req {
	char* host;
	int port;
	char* user;
	char* pass;
	char* db_name;
	void(*open_cb)(const char* err, void* context);
	void* context;
	char* err;
};

struct mysql_context {
	void* context;
	uv_mutex_t lock;
	int is_closing;
};

static void connect_work(uv_work_t* req) {
	struct connect_req* connect_req = (struct connect_req*)req->data;
	sql::Driver* driver = get_driver_instance();

	static const char* url_format = "tcp://%s:%d";
	char url[128];
	sprintf(url, url_format, connect_req->host, connect_req->port);

	sql::Connection* con = NULL;

	try
	{
		con = driver->connect("127.0.0.1", "root", "123");

		if (!con || !con->isValid())
		{
			connect_req->context = NULL;
			connect_req->err = (char*)"connect failed";
			return;
		}
	}
	catch (const std::exception&)
	{
		connect_req->context = NULL;
		connect_req->err = (char*)"connect failed";
		return;
	}
	
	con->setSchema(connect_req->db_name);

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
		connect_req->open_cb(connect_req->err, connect_req->context);
	}

	if (connect_req->host) {
		free(connect_req->host);
		connect_req->host = NULL;  // Nullify the pointer after freeing
	}
	if (connect_req->db_name) {
		free(connect_req->db_name);
		connect_req->db_name = NULL;
	}
	if (connect_req->user) {
		free(connect_req->user);
		connect_req->user = NULL;
	}
	if (connect_req->pass) {
		free(connect_req->pass);
		connect_req->pass = NULL;
	}
	if (connect_req->err) {
		free(connect_req->err);
		connect_req->err = NULL;
	}
	if (req->data) {
		free(req->data);
		req->data = NULL;  // Nullify req->data to prevent double free
	}

	free(req);
}

void mysql_wrapper::connect(const char* host, const int port, const char* user, const char* pass, const char* db_name, void(*open_cb)(const char* err, void* context))
{
	uv_work_t* req = (uv_work_t*)my_malloc(sizeof(uv_work_t));
	memset(req, 0, sizeof(uv_work_t));

	struct connect_req* connect_req = (struct connect_req*)my_malloc(sizeof(struct connect_req));
	memset(connect_req, 0, sizeof(struct connect_req));

	connect_req->host = strdup(host);
	connect_req->port = port;
	connect_req->user = strdup(user);
	connect_req->pass = strdup(pass);
	connect_req->db_name = strdup(db_name);
	connect_req->open_cb = open_cb;
	connect_req->err = NULL;

	req->data = (void*)connect_req;

	uv_queue_work(uv_default_loop(), req, connect_work, on_connect_work_complete);
}

struct query_req {
	void* context;
	char* sql;
	void(*query_cb)(const char* err, std::vector<std::vector<std::string>>* result);
	std::vector<std::vector<std::string>>* result;
	char* err;
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

		r->result = new std::vector<std::vector<std::string>>();

		int r_size = 0;
		while (res->next()) {
			std::vector<std::string> row;
			for (int i = 1; i <= res->getMetaData()->getColumnCount(); i++) {
				row.push_back(res->getString(i));
			}
			((std::vector<std::vector<std::string>>*)(r->result))->push_back(row);
			r_size = 1;
		}
		if (!r_size)
			r->result = NULL;

		delete res;
		delete stmt;
	}
	catch (const std::exception& e)
	{
		r->result = NULL;
		r->err = (char*)e.what();
	}

	uv_mutex_unlock(&my_conn->lock);
}

static void on_query_work_complete(uv_work_t* req, int status) {
	query_req* r = (query_req*)req->data;
	r->query_cb(r->err, r->result);
	
	if (r->sql)
		free(r->sql);
	if (r->result)
		delete r->result;
	if (r->err)
		free(r->err);

	my_free(r);
	my_free(req);
}


void mysql_wrapper::query(void* context, const char* sql, void(*query_cb)(const char* err, std::vector<std::vector<std::string>>* result))
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
	r->sql = strdup(sql);
	r->query_cb = query_cb;

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

void mysql_wrapper::close(void* context)
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