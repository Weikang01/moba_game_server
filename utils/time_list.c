#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <uv.h>
#include "time_list.h"

#define MALLOC malloc
#define FREE free

struct timer_data {
	uv_timer_t timer;  // libuv timer
	void(*callback)(void*);
	void* data;
	int repeat;  // -1 for infinite
};

static struct timer_data* alloc_timer(void(*callback)(void*), void* data, int repeat) {
	struct timer_data* timer = (struct timer_data*)MALLOC(sizeof(struct timer_data));
	if (timer == NULL) {
		return NULL;
	}
	memset(timer, 0, sizeof(struct timer_data));
	timer->callback = callback;
	timer->data = data;
	timer->repeat = repeat;
	uv_timer_init(uv_default_loop(), &timer->timer);
	return timer;
}

static void free_timer(struct timer_data* timer) {
	FREE(timer);
}

static void timer_callback(uv_timer_t* handle) {
	struct timer_data* timer = (struct timer_data*)handle->data;
	if (timer->repeat > 0) {
		timer->callback(timer->data);
		timer->repeat--;
	}
	else if (timer->repeat == 0) {
		uv_timer_stop(&timer->timer);
		free_timer(timer);
	}
	else {
		timer->callback(timer->data);
	}
}


struct timer_data* schedule(void(*callback)(void*), void* data, unsigned int after_ms, unsigned int repeat_interval_ms, int repeat)
{
	struct timer_data* timer = alloc_timer(callback, data, repeat);
	timer->timer.data = timer;
	uv_timer_start(&timer->timer, timer_callback, after_ms, repeat_interval_ms);

	return timer;
}

void unschedule(struct timer_data* timer)
{
	if (timer->repeat == 0) {
		return;
	}
	uv_timer_stop(&timer->timer);
	free_timer(timer);
}

struct timer_data* schedule_once(void(*callback)(void*), void* data, unsigned int after_ms)
{
	return schedule(callback, data, after_ms, 1000, 1);
}



