#ifndef __TIME_LIST_H__
#define __TIME_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

	struct timer_data;

	struct timer_data* schedule(void(*callback)(void*), void* data, unsigned int after_ms, unsigned int repeat_interval_ms, int repeat);

	void unschedule(struct timer_data* timer);

	struct timer_data* schedule_once(void(*callback)(void*), void* data, unsigned int after_ms);

#ifdef __cplusplus
}
#endif

#endif