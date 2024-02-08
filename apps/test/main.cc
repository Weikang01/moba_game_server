#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include <iostream>
#include <string>
#include <vector>
#include <hiredis.h>

#include "../../utils/logger.h"
#include "../../utils/time_list.h"
#include "../../utils/timestamp.h"
#include "../../netbus/proto_manager.h"
#include "../../netbus/netbus.h"
#include "proto/pf_cmd_map.h"
#include "../../database/mysql_wrapper.h"
#include "../../database/redis_wrapper.h"
#include "../../lua_wrapper/lua_wrapper.h"



static void on_logger_timer(void* user_data) {
	log_debug("on_logger_timer");
}

static void test_on_mysql_query(const char* erro, sql::ResultSet* result, void* udata) {
	printf("test_on_mysql_query\n");
	if (erro != NULL) {
		log_error("test_on_mysql_query error: %s", erro);
		return;
	}

	if (result) {
		while (result->next()) {
			log_debug("test_on_mysql_query: %s, %d", result->getString("name").c_str(), result->getInt("family_size"));
		}
	}
	else {
		log_debug("test_on_mysql_query: no result");
	}
}

static void test_on_mysql_connect(const char* erro, void* context, void* udata) {
	printf("test_on_mysql_connect\n");

	if (erro != NULL) {
		log_error("test_on_mysql_connect error: %s", erro);
		return;
	}

	mysql_wrapper::query(context, "insert into families (`name`, `family_size`) values('alex2', 12);", test_on_mysql_query);
}

static void test_mysql() {
	mysql_wrapper::connect("127.0.0.1", 3306, "root", "123", "test_database", test_on_mysql_connect);
}

static void test_on_redis_query(const char* erro, redisReply* result, void* udata) {
	printf("test_on_redis_query\n");
	if (erro != NULL) {
		log_error("test_on_redis_query error: %s", erro);
		return;
	}

	if (result) {
		log_debug("test_on_redis_query: %s", result->str);
	}
	else {
		log_debug("test_on_redis_query: no result");
	}
}

static void test_on_redis_connect(const char* erro, void* context, void* udata) {
	printf("test_on_redis_connect\n");
	if (erro != NULL) {
		log_error("test_on_redis_connect error: %s", erro);
		return;
	}

	const char* name = "alex";
	const char* sql = "set test32 %s";
	char* sql2 = (char*)malloc(strlen(name) + strlen(sql) + 1);
	sprintf(sql2, sql, name);

	redis_wrapper::query(context, sql2, NULL);

	redis_wrapper::query(context, "get test32", test_on_redis_query);
}

static void test_redis() {
	redis_wrapper::connect("127.0.0.1", 6379, test_on_redis_connect);
}

int main(int argc, char** argv) {
	proto_manager::init(PROTO_BUF);
	pf_cmd_map_init();
	//logger::init("log/test", "netbus_log", true, LOG_TIMEZONE_EST);

	//test_mysql();
	//test_redis();

	netbus::instance()->init();

	//netbus::instance()->start_tcp_server(8023);
	//netbus::instance()->start_ws_server(8043);
	//netbus::instance()->start_udp_server(8063);

	// initiate script
	lua_wrapper::init();
	lua_wrapper::execute_script_file("./main.lua");

	netbus::instance()->run();

	lua_wrapper::exit();
  return 0;
}
