#ifndef  __TIMESTAMP_H__
#define __TIMESTAMP_H__


#ifdef __cplusplus
extern "C" {
#endif
	// get current timestamp
	unsigned long timestamp();

	unsigned long date2timestamp(const char* fmt_date, const char* date);

	void timestamp2date(unsigned long timestamp, char* fmt_date, char* out_date, int out_len);

	unsigned long timestamp_today();

#ifdef __cplusplus
}
#endif

#endif // ! __TIMESTAMP_H__
