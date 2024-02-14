#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lua_wrapper/lua_wrapper.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <tolua++.h>
#ifdef __cplusplus
}
#endif

#include <tolua_fix.h>

#include "scheduler_export_to_lua.h"

#include "../utils/time_list.h"
#include "../utils/small_alloc.h"

#define my_malloc small_alloc
#define my_free small_free


struct lua_schedule_data {
    int handle;
    int repeat;
};

static void lua_scheduler_callback(void* data)
{
    struct lua_schedule_data* timer = (struct lua_schedule_data*)data;
    LuaWrapper::execute_function_by_handler(timer->handle, 0);

    if (timer->repeat == -1)
        return;

    timer->repeat--;
    if (timer->repeat == 0)
    {
        LuaWrapper::remove_script_handler(timer->handle);
        my_free(timer);
	}
}

static int lua_scheduler_schedule(lua_State* tolua_S)
{
    int handle = toluafix_ref_function(tolua_S, 1, 0);
    int after_ms, repeat_interval_ms, repeat;
    struct lua_schedule_data* timer;
    timer_data* td;
    if (handle == 0)
		goto lua_failed;

    after_ms = tolua_tonumber(tolua_S, 2, 0);
    if (after_ms < 0)
        goto lua_failed;

    repeat_interval_ms = tolua_tonumber(tolua_S, 3, 0);
    if (repeat_interval_ms < 0)
		goto lua_failed;

    repeat = tolua_tonumber(tolua_S, 4, 0);
    if (repeat < 0)
        repeat = -1;  // repeat forever

    timer = (struct lua_schedule_data*)my_malloc(sizeof(struct lua_schedule_data));
    timer->handle = handle;
    timer->repeat = repeat;

    td = schedule(lua_scheduler_callback, (void*)timer, after_ms, repeat_interval_ms, repeat);
    tolua_pushuserdata(tolua_S, td);

    return 1;
lua_failed:
    if (handle != 0)
		LuaWrapper::remove_script_handler(handle);
    lua_pushnil(tolua_S);
	return 0;
}

static int lua_scheduler_unschedule(lua_State* tolua_S)
{
    if (!lua_isuserdata(tolua_S, 1))
		return 0;
    timer_data* td = (timer_data*)tolua_touserdata(tolua_S, 1, 0);
	unschedule(td);
	return 0;
}

static void lua_scheduler_once_callback(void* data)
{
    struct lua_schedule_data* timer = (struct lua_schedule_data*)data;
    LuaWrapper::execute_function_by_handler(timer->handle, 0);
    LuaWrapper::remove_script_handler(timer->handle);
    my_free(timer);
}

static int lua_scheduler_schedule_once(lua_State* tolua_S)
{
    int handle = toluafix_ref_function(tolua_S, 1, 0);
    int after_ms, repeat_interval_ms, repeat;
    struct lua_schedule_data* timer;
    timer_data* td;
    if (handle == 0)
        goto lua_failed;

    after_ms = tolua_tonumber(tolua_S, 2, 0);
    if (after_ms <= 0)
        goto lua_failed;

    repeat_interval_ms = tolua_tonumber(tolua_S, 3, 0);
    if (repeat_interval_ms < 0)
        goto lua_failed;

    timer = (struct lua_schedule_data*)my_malloc(sizeof(struct lua_schedule_data));
    timer->handle = handle;
    timer->repeat = 1;

    td = schedule_once(lua_scheduler_once_callback, (void*)timer, after_ms);

    tolua_pushuserdata(tolua_S, td);
    return 1;
lua_failed:
    if (handle != 0)
        LuaWrapper::remove_script_handler(handle);

    lua_pushnil(tolua_S);
    return 0;
}

int register_scheduler_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "Scheduler", 0);
        tolua_beginmodule(tolua_S, "Scheduler");

        tolua_function(tolua_S, "schedule", lua_scheduler_schedule);
        tolua_function(tolua_S, "cancel", lua_scheduler_unschedule);
        tolua_function(tolua_S, "once", lua_scheduler_schedule_once);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
