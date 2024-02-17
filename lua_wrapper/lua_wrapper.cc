#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>

#include <lua.hpp>
#include <tolua_fix.h>

#include <google/protobuf/message.h>


#include "lua_wrapper.h"

#include "../utils/logger.h"
#include "logger_export_to_lua.h"
#include "mysql_export_to_lua.h"
#include "redis_export_to_lua.h"
#include "service_export_to_lua.h"
#include "session_export_to_lua.h"
#include "scheduler_export_to_lua.h"
#include "netbus_export_to_lua.h"
#include "proto_manager_export_to_lua.h"

using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;

lua_State* g_lua_state = NULL;

extern void print_error(const char* file_name, int line_num, const char* msg);
extern void do_log_message(void(*log)(const char* file_name, int line_num, const char* msg), const char* msg);

void push_proto_message_tolua(Message* message)
{
	lua_State* state = LuaWrapper::get_lua_state();
	if (!message) {
		// printf("PushProtobuf2LuaTable failed, message is NULL");
		return;
	}
	const Reflection* reflection = message->GetReflection();

	// create a new table
	lua_newtable(state);

	const Descriptor* descriptor = message->GetDescriptor();
	for (int32_t index = 0; index < descriptor->field_count(); ++index) {
		const FieldDescriptor* field = descriptor->field(index);
		const std::string& name = field->lowercase_name();

		lua_pushstring(state, name.c_str());

		bool bReapeted = field->is_repeated();

		if (bReapeted) {
			lua_newtable(state);
			int size = reflection->FieldSize(*message, field);
			for (int i = 0; i < size; ++i) {
				if (field->cpp_type() == FieldDescriptor::CPPTYPE_MESSAGE) {
					const Message& sub_message = reflection->GetRepeatedMessage(*message, field, i);
					push_proto_message_tolua((Message*)&sub_message);
				}
				else {
					char str[32] = { 0 };
					switch (field->cpp_type()) {
					case FieldDescriptor::CPPTYPE_DOUBLE:
						lua_pushnumber(state, reflection->GetRepeatedDouble(*message, field, i));
						break;
					case FieldDescriptor::CPPTYPE_FLOAT:
						lua_pushnumber(state, (double)reflection->GetRepeatedFloat(*message, field, i));
						break;
					case FieldDescriptor::CPPTYPE_INT64:
						sprintf(str, "%lld", (long long)reflection->GetRepeatedInt64(*message, field, i));
						lua_pushstring(state, str);
						break;
					case FieldDescriptor::CPPTYPE_UINT64:

						sprintf(str, "%llu", (unsigned long long)reflection->GetRepeatedUInt64(*message, field, i));
						lua_pushstring(state, str);
						break;
					case FieldDescriptor::CPPTYPE_ENUM:
						lua_pushinteger(state, reflection->GetRepeatedEnum(*message, field, i)->number());
						break;
					case FieldDescriptor::CPPTYPE_INT32:
						lua_pushinteger(state, reflection->GetRepeatedInt32(*message, field, i));
						break;
					case FieldDescriptor::CPPTYPE_UINT32:
						lua_pushinteger(state, reflection->GetRepeatedUInt32(*message, field, i));
						break;
					case FieldDescriptor::CPPTYPE_STRING:
					{
						std::string value = reflection->GetRepeatedString(*message, field, i);
						lua_pushlstring(state, value.c_str(), value.size());
					}
					break;
					case FieldDescriptor::CPPTYPE_BOOL:
						lua_pushboolean(state, reflection->GetRepeatedBool(*message, field, i));
						break;
					case FieldDescriptor::CPPTYPE_MESSAGE:
						push_proto_message_tolua((Message*)&reflection->GetRepeatedMessage(*message, field, i));
						break;
					default:
						break;
					}
				}
				lua_rawseti(state, -2, i + 1); // lua's index start at 1
			}
		}
		else {
			char str[32] = { 0 };
			switch (field->cpp_type())
			{
			case FieldDescriptor::CPPTYPE_DOUBLE:
				lua_pushnumber(state, reflection->GetDouble(*message, field));
				break;
			case FieldDescriptor::CPPTYPE_FLOAT:
				lua_pushnumber(state, (double)reflection->GetFloat(*message, field));
				break;
			case FieldDescriptor::CPPTYPE_INT64:

				sprintf(str, "%lld", (long long)reflection->GetInt64(*message, field));
				lua_pushstring(state, str);
				break;
			case FieldDescriptor::CPPTYPE_UINT64:

				sprintf(str, "%llu", (unsigned long long)reflection->GetUInt64(*message, field));
				lua_pushstring(state, str);
				break;
			case FieldDescriptor::CPPTYPE_ENUM: // 与int32一样处理
				lua_pushinteger(state, (int)reflection->GetEnum(*message, field)->number());
				break;
			case FieldDescriptor::CPPTYPE_INT32:
				lua_pushinteger(state, reflection->GetInt32(*message, field));
				break;
			case FieldDescriptor::CPPTYPE_UINT32:
				lua_pushinteger(state, reflection->GetUInt32(*message, field));
				break;
			case FieldDescriptor::CPPTYPE_STRING:
			{
				std::string value = reflection->GetString(*message, field);
				lua_pushlstring(state, value.c_str(), value.size());
			}
			break;
			case FieldDescriptor::CPPTYPE_BOOL:
				lua_pushboolean(state, reflection->GetBool(*message, field));
				break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
				push_proto_message_tolua((Message*)&reflection->GetMessage(*message, field));
				break;
			default:
				break;
			}
		}

		lua_rawset(state, -3);
	}
}

static int lua_panic(lua_State* L) {
	const char* err = lua_tostring(L, -1);
	if (err) {
		do_log_message(print_error, err);
	}
	return 0;
}

void LuaWrapper::register_function(const char* name, lua_CFunction func)
{
	lua_pushcfunction(g_lua_state, func);
	lua_setglobal(g_lua_state, name);
}

static int lua_add_search_path(lua_State* L) {
	const char* path = luaL_checkstring(L, 1);
	if (path) {
		LuaWrapper::add_search_path(path);
	}
	return 0;
}

void LuaWrapper::init()
{
	g_lua_state = luaL_newstate();
	lua_atpanic(g_lua_state, lua_panic);
	luaL_openlibs(g_lua_state);
	toluafix_open(g_lua_state);
	LuaWrapper::register_function("add_search_path", lua_add_search_path);

	register_logger_export(g_lua_state);
	register_mysql_export(g_lua_state);
	register_redis_export(g_lua_state);
	register_service_export(g_lua_state);
	register_session_export(g_lua_state);
	register_scheduler_export(g_lua_state);
	register_netbus_export(g_lua_state);
	register_proto_manager_export(g_lua_state);
	register_raw_cmd_export(g_lua_state);
}

void LuaWrapper::exit()
{
	if (g_lua_state)
	{
		lua_close(g_lua_state);
		g_lua_state = NULL;
	}
}

bool LuaWrapper::execute_script_file(const std::string& file_path)
{
	if (luaL_dofile(g_lua_state, file_path.c_str()))
	{
		const char* error_string = lua_tostring(g_lua_state, -1);
		log_error("execute_script_file error: %s", error_string);
		lua_pop(g_lua_state, 1);
		return false;
	}
	return true;
}

lua_State* LuaWrapper::get_lua_state()
{
	return g_lua_state;
}

static bool pushFunctionByHandler(int nHandler)
{
	toluafix_get_function_by_refid(g_lua_state, nHandler);
	if (!lua_isfunction(g_lua_state, -1))
	{
		log_error("function refid '%d' does not reference a Lua function", nHandler);
		lua_pop(g_lua_state, 1);
		return false;
	}
	return true;
}

static int executeFunction(int numArgs)
{
	int functionIndex = -(numArgs + 1);
	if (!lua_isfunction(g_lua_state, functionIndex))
	{
		log_error("value at stack [%d] is not function", functionIndex);
		lua_pop(g_lua_state, numArgs + 1); // remove function and arguments
		return 0;
	}

	int traceback = 0;
	lua_getglobal(g_lua_state, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
	if (!lua_isfunction(g_lua_state, -1))
	{
		lua_pop(g_lua_state, 1);                                            /* L: ... func arg1 arg2 ... */
	}
	else
	{
		lua_insert(g_lua_state, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
		traceback = functionIndex - 1;
	}

	int error = 0;
	error = lua_pcall(g_lua_state, numArgs, 1, traceback);                  /* L: ... [G] ret */
	if (error)
	{
		if (traceback == 0)
		{
			log_error("[LUA ERROR] %s", lua_tostring(g_lua_state, -1));        /* L: ... error */
			lua_pop(g_lua_state, 1); // remove error message from stack
		}
		else                                                            /* L: ... G error */
		{
			lua_pop(g_lua_state, 2); // remove __G__TRACKBACK__ and error message from stack
		}
		return 0;
	}

	// get return value
	int ret = 0;
	if (lua_isnumber(g_lua_state, -1))
	{
		ret = (int)lua_tointeger(g_lua_state, -1);
	}
	else if (lua_isboolean(g_lua_state, -1))
	{
		ret = (int)lua_toboolean(g_lua_state, -1);
	}
	// remove return value from stack
	lua_pop(g_lua_state, 1);                                                /* L: ... [G] */

	if (traceback)
	{
		lua_pop(g_lua_state, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
	}

	return ret;
}

int LuaWrapper::execute_function_by_handler(int handler, int nargs)
{
	int ret = 0;
	if (pushFunctionByHandler(handler))
	{
		if (nargs > 0)
		{
			lua_insert(g_lua_state, -nargs - 1);
		}
		ret = executeFunction(nargs);
	}
	lua_settop(g_lua_state, 0);
	return ret;
}

void LuaWrapper::remove_script_handler(int handler)
{
	toluafix_remove_function_by_refid(g_lua_state, handler);
}

void LuaWrapper::add_search_path(const std::string& path)
{
	char script[1024] = { 0 };
	sprintf(script, "local path = string.match([[%s]],[[(.*)/[^/]*$]])\n package.path = package.path .. [[;]] .. path .. [[/?.lua;]] .. path .. [[/?/init.lua]]\n", path.c_str());
	luaL_dostring(g_lua_state, script);
}
