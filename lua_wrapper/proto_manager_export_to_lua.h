#ifndef __PROTO_MANAGER_EXPORT_TO_LUA_H__
#define __PROTO_MANAGER_EXPORT_TO_LUA_H__

struct lua_State;

int register_proto_manager_export(lua_State* L);
int register_raw_cmd_export(lua_State* L);

#endif // !__NETBUS_EXPORT_TO_LUA_H__
