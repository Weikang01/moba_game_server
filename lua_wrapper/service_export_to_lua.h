#ifndef __SERVICE_EXPORT_TO_LUA_H__
#define __SERVICE_EXPORT_TO_LUA_H__

struct lua_State;

int register_service_export(lua_State* L);

#endif // !__SERVICE_EXPORT_TO_LUA_H__
