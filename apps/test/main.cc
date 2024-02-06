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



static void on_logger_timer(void* user_data) {
	log_debug("on_logger_timer");
}

static void test_on_mysql_query(const char* erro, std::vector<std::vector<std::string>>* result) {
	printf("test_on_mysql_query\n");
	if (erro != NULL) {
		log_error("test_on_mysql_query error: %s", erro);
		return;
	}

	if (result) {
		for (size_t i = 0; i < result->size(); i++) {
			std::vector<std::string>& row = (*result)[i];
			for (size_t j = 0; j < row.size(); j++) {
				log_debug("test_on_mysql_query: %s", row[j].c_str());
			}
		}
	}
}

static void test_on_mysql_connect(const char* erro, void* context) {
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

static void test_on_redis_query(const char* erro, void* result, int result_type) {
	printf("test_on_redis_query\n");
	if (erro != NULL) {
		log_error("test_on_redis_query error: %s", erro);
		return;
	}
	redisReply* reply = (redisReply*)result;

	if (result) {
		if (result_type == REDIS_REPLY_STRING) {
			log_debug("test_on_redis_query: %s", (char*)result);
		}
		else if (result_type == REDIS_REPLY_ARRAY) {
			for (size_t i = 0; i < reply->elements; i++) {
				log_debug("test_on_redis_query: %s", reply->element[i]->str);
			}
		}
		else {
			log_debug("test_on_redis_query: result_type: %d", result_type);
		}
	}
	else {
		log_debug("test_on_redis_query: result is NULL");
	}
}

static void test_on_redis_connect(const char* erro, void* context) {
	printf("test_on_redis_connect\n");
	if (erro != NULL) {
		log_error("test_on_redis_connect error: %s", erro);
		return;
	}

	const char* name = "alex";
	redis_wrapper::query(context, NULL, "set test32 %s", name);

	redis_wrapper::query(context, test_on_redis_query, "get test32");
}

static void test_redis() {
	redis_wrapper::connect("127.0.0.1", 6379, test_on_redis_connect);
}

int main(int argc, char** argv) {
	proto_manager::init(PROTO_BUF);
	pf_cmd_map_init();
	logger::init("log/test", "netbus_log", true, LOG_TIMEZONE_EST);

	log_debug("test log_debug");

	//test_mysql();
	//test_redis();

	netbus::instance()->init();

	//netbus::instance()->start_tcp_server(8023);
	//netbus::instance()->start_ws_server(8043);
	//netbus::instance()->start_udp_server(8063);

	netbus::instance()->run();
  return 0;
}
