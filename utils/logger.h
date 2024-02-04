#ifndef __LOGGER_H__
#define __LOGGER_H__

enum {
	LOG_DEBUG = 0,
	LOG_WARN = 1,
	LOG_ERROR = 2,
};

#define log_debug(msg, ...) logger::log(__FILE__, __LINE__, LOG_DEBUG, msg, ##__VA_ARGS__)
#define log_warn(msg, ...) logger::log(__FILE__, __LINE__, LOG_WARN, msg, ##__VA_ARGS__)
#define log_error(msg, ...) logger::log(__FILE__, __LINE__, LOG_ERROR, msg, ##__VA_ARGS__)

using TIMEZONE = int;
constexpr TIMEZONE LOG_TIMEZONE_EST = -5;
constexpr TIMEZONE LOG_TIMEZONE_CST = -6;
constexpr TIMEZONE LOG_TIMEZONE_MST = -7;
constexpr TIMEZONE LOG_TIMEZONE_PST = -8;
constexpr TIMEZONE LOG_TIMEZONE_GMT = 0;
constexpr TIMEZONE LOG_TIMEZONE_UTC = 0;
constexpr TIMEZONE LOG_TIMEZONE_CET = 1;
constexpr TIMEZONE LOG_TIMEZONE_EET = 2;
constexpr TIMEZONE LOG_TIMEZONE_JST = 9;
constexpr TIMEZONE LOG_TIMEZONE_AEST = 10;


class logger {
public:
	static void init(const char* path, const char* prefix, bool std_output = false, TIMEZONE time_zone= LOG_TIMEZONE_UTC);
	static void log(const char* file_name,
		int line_num,
		int level, const char* msg, ...);
};



#endif // !__LOGGER_H__
