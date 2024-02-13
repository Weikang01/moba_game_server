#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <tolua++.h>
#ifdef __cplusplus
}
#endif
#include <tolua_fix.h>

#include "../netbus/proto_manager.h"
#include "session_export_to_lua.h"
#include "../netbus/session.h"
#include "../utils/logger.h"

int lua_session_close(lua_State* tolua_S)
{
    session* s = (session*)tolua_touserdata(tolua_S, 1, 0);

    if (s)
		s->close();
	return 0;
}


using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;
using google::protobuf::EnumDescriptor;
using google::protobuf::EnumValueDescriptor;

static google::protobuf::Message* lua_table_to_protobuf(lua_State* L, int stack_index, const char* msg_name)
{
    Message* message = proto_manager::create_message(msg_name);
    if (!message) {
        log_error("cant find message  %s from compiled poll \n", msg_name);
        return NULL;
    }

	const Reflection* reflection = message->GetReflection();
	const Descriptor* descriptor = message->GetDescriptor();

	for (int32_t index = 0; index < descriptor->field_count(); ++index) {
		const FieldDescriptor* fd = descriptor->field(index);
		const std::string& name = fd->name();

		bool isRequired = fd->is_required();
		bool bReapeted = fd->is_repeated();
		lua_pushstring(L, name.c_str());
		lua_rawget(L, stack_index);

		bool isNil = lua_isnil(L, -1);

		if (bReapeted) {
			if (isNil) {
				lua_pop(L, 1);
				continue;
			}
			else {
				bool isTable = lua_istable(L, -1);
				if (!isTable) {
					log_error("can't find required repeated field %s\n", name.c_str());
					proto_manager::free_protobuf_message(message);
					return NULL;
				}
			}

			lua_pushnil(L);
			for (; lua_next(L, -2) != 0;) {
				switch (fd->cpp_type()) {

				case FieldDescriptor::CPPTYPE_DOUBLE:
				{
					double value = luaL_checknumber(L, -1);
					reflection->AddDouble(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_FLOAT:
				{
					float value = luaL_checknumber(L, -1);
					reflection->AddFloat(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_INT64:
				{
					int64_t value = luaL_checknumber(L, -1);
					reflection->AddInt64(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_UINT64:
				{
					uint64_t value = luaL_checknumber(L, -1);
					reflection->AddUInt64(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_ENUM:
				{
					int32_t value = luaL_checknumber(L, -1);
					const EnumDescriptor* enumDescriptor = fd->enum_type();
					const EnumValueDescriptor* valueDescriptor = enumDescriptor->FindValueByNumber(value);
					reflection->AddEnum(message, fd, valueDescriptor);
				}
				break;
				case FieldDescriptor::CPPTYPE_INT32:
				{
					int32_t value = luaL_checknumber(L, -1);
					reflection->AddInt32(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_UINT32:
				{
					uint32_t value = luaL_checknumber(L, -1);
					reflection->AddUInt32(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_STRING:
				{
					size_t size = 0;
					const char* value = luaL_checklstring(L, -1, &size);
					reflection->AddString(message, fd, std::string(value, size));
				}
				break;
				case FieldDescriptor::CPPTYPE_BOOL:
				{
					bool value = lua_toboolean(L, -1);
					reflection->AddBool(message, fd, value);
				}
				break;
				case FieldDescriptor::CPPTYPE_MESSAGE:
				{
					Message* value = lua_table_to_protobuf(L, lua_gettop(L), fd->message_type()->name().c_str());
					if (!value) {
						log_error("convert to message %s failed whith value %s\n", fd->message_type()->name().c_str(), name.c_str());
						proto_manager::free_protobuf_message(value);
						return NULL;
					}
					Message* msg = reflection->AddMessage(message, fd);
					msg->CopyFrom(*value);
					proto_manager::free_protobuf_message(value);
				}
				break;
				default:
					break;
				}

				// remove value£¬ keep the key
				lua_pop(L, 1);
			}
		}
		else {

			if (isRequired) {
				if (isNil) {
					log_error("cant find required field %s\n", name.c_str());
					proto_manager::free_protobuf_message(message);
					return NULL;
				}
			}
			else {
				if (isNil) {
					lua_pop(L, 1);
					continue;
				}
			}

			switch (fd->cpp_type()) {
			case FieldDescriptor::CPPTYPE_DOUBLE:
			{
				double value = luaL_checknumber(L, -1);
				reflection->SetDouble(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_FLOAT:
			{
				float value = luaL_checknumber(L, -1);
				reflection->SetFloat(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_INT64:
			{
				int64_t value = luaL_checknumber(L, -1);
				reflection->SetInt64(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_UINT64:
			{
				uint64_t value = luaL_checknumber(L, -1);
				reflection->SetUInt64(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_ENUM:
			{
				int32_t value = luaL_checknumber(L, -1);
				const EnumDescriptor* enumDescriptor = fd->enum_type();
				const EnumValueDescriptor* valueDescriptor = enumDescriptor->FindValueByNumber(value);
				reflection->SetEnum(message, fd, valueDescriptor);
			}
			break;
			case FieldDescriptor::CPPTYPE_INT32:
			{
				int32_t value = luaL_checknumber(L, -1);
				reflection->SetInt32(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_UINT32:
			{
				uint32_t value = luaL_checknumber(L, -1);
				reflection->SetUInt32(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_STRING:
			{
				size_t size = 0;
				const char* value = luaL_checklstring(L, -1, &size);
				reflection->SetString(message, fd, std::string(value, size));
			}
			break;
			case FieldDescriptor::CPPTYPE_BOOL:
			{
				bool value = lua_toboolean(L, -1);
				reflection->SetBool(message, fd, value);
			}
			break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
			{
				Message* value = lua_table_to_protobuf(L, lua_gettop(L), fd->message_type()->name().c_str());
				if (!value) {
					log_error("convert to message %s failed whith value %s \n", fd->message_type()->name().c_str(), name.c_str());
					proto_manager::free_protobuf_message(message);
					return NULL;
				}
				Message* msg = reflection->MutableMessage(message, fd);
				msg->CopyFrom(*value);
				proto_manager::free_protobuf_message(value);
			}
			break;
			default:
				break;
			}
		}

		// pop value
		lua_pop(L, 1);
	}

    return message;
}


/*
struct cmd_msg {
    int stype; // service type
    int ctype; // cmd type
    int utag; // user tag
    void* body; // json string or binary buffer
};
*/
int lua_session_send_msg(lua_State* tolua_S)
{
    session* s = (session*)tolua_touserdata(tolua_S, 1, 0);
    int stype, ctype, utag;
    if (!s)
        goto failed;

    // stack: 1. session, 2. table
    if (!lua_istable(tolua_S, 2))
        goto failed;

    lua_getfield(tolua_S, 2, "stype");
    lua_getfield(tolua_S, 2, "ctype");
    lua_getfield(tolua_S, 2, "utag");
    lua_getfield(tolua_S, 2, "body");

    struct cmd_msg msg;

    msg.stype = lua_tointeger(tolua_S, -4);
    msg.ctype = lua_tointeger(tolua_S, -3);
    msg.utag  = lua_tointeger(tolua_S, -2);

    if (proto_manager::proto_type() == PROTO_JSON)
    {
        msg.body = (char*)lua_tostring(tolua_S, -1);
        s->send_msg(&msg);
    }
    else if (proto_manager::proto_type() == PROTO_BUF)
    {
		if (!lua_istable(tolua_S, -1)) {
			msg.body = NULL;
			s->send_msg(&msg);
		}
        else {
            const char* msg_name = proto_manager::pb_type2name(msg.ctype);
            msg.body = lua_table_to_protobuf(tolua_S, lua_gettop(tolua_S), msg_name);
			s->send_msg(&msg);
			proto_manager::free_protobuf_message((Message*)msg.body);
        }
    }

failed:
	return 0;
}

int lua_session_get_address(lua_State* tolua_S)
{
    session* s = (session*)tolua_touserdata(tolua_S, 1, 0);
    int port;
    const char* addr;
	if (!s)
        goto failed;

    addr = s->get_address(&port);

    lua_pushstring(tolua_S, addr);
    lua_pushinteger(tolua_S, port);

    return 2;

failed:
	return 0;
}

int register_session_export(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "session", 0);
        tolua_beginmodule(tolua_S, "session");

        tolua_function(tolua_S, "close", lua_session_close);
        tolua_function(tolua_S, "send_msg", lua_session_send_msg);
        tolua_function(tolua_S, "get_address", lua_session_get_address);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
