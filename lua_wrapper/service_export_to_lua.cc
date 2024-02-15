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
#include <google/protobuf/message.h>

#include "../lua_wrapper/lua_wrapper.h"

#include "../netbus/session.h"
#include "service_export_to_lua.h"
#include "../netbus/service.h"
#include "../netbus/proto_manager.h"
#include "../netbus/service_manager.h"
#include "../utils/logger.h"

using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::Reflection;

#define SERVICE_FUNCTION_MAPPING "service_function_mapping"

static int exe_function(int numArgs)
{
    int functionIndex = -(numArgs + 1);
    if (!lua_isfunction(LuaWrapper::get_lua_state(), functionIndex))
    {
        log_error("value at stack [%d] is not function", functionIndex);
        lua_pop(LuaWrapper::get_lua_state(), numArgs + 1); // remove function and arguments
        return 0;
    }

    int traceback = 0;
    lua_getglobal(LuaWrapper::get_lua_state(), "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(LuaWrapper::get_lua_state(), -1))
    {
        lua_pop(LuaWrapper::get_lua_state(), 1);                                            /* L: ... func arg1 arg2 ... */
    }
    else
    {
        lua_insert(LuaWrapper::get_lua_state(), functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
        traceback = functionIndex - 1;
    }

    int error = 0;
    error = lua_pcall(LuaWrapper::get_lua_state(), numArgs, 1, traceback);                  /* L: ... [G] ret */
    if (error)
    {
        if (traceback == 0)
        {
            log_error("[LUA ERROR] %s", lua_tostring(LuaWrapper::get_lua_state(), -1));        /* L: ... error */
            lua_pop(LuaWrapper::get_lua_state(), 1); // remove error message from stack
        }
        else                                                            /* L: ... G error */
        {
            lua_pop(LuaWrapper::get_lua_state(), 2); // remove __G__TRACKBACK__ and error message from stack
        }
        return 0;
    }

    // get return value
    int ret = 0;
    if (lua_isnumber(LuaWrapper::get_lua_state(), -1))
    {
        ret = (int)lua_tointeger(LuaWrapper::get_lua_state(), -1);
    }
    else if (lua_isboolean(LuaWrapper::get_lua_state(), -1))
    {
        ret = (int)lua_toboolean(LuaWrapper::get_lua_state(), -1);
    }
    // remove return value from stack
    lua_pop(LuaWrapper::get_lua_state(), 1);                                                /* L: ... [G] */

    if (traceback)
    {
        lua_pop(LuaWrapper::get_lua_state(), 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
    }

    return ret;
}

static void get_service_function(lua_State* L, int refid)
{
    lua_pushstring(L, SERVICE_FUNCTION_MAPPING);
    lua_rawget(L, LUA_REGISTRYINDEX);                           /* stack: ... refid_fun */
    lua_pushinteger(L, refid);                                  /* stack: ... refid_fun refid */
    lua_rawget(L, -2);                                          /* stack: ... refid_fun fun */
    lua_remove(L, -2);                                          /* stack: ... fun */
}

static bool push_service_function(int nHandler)
{
    get_service_function(LuaWrapper::get_lua_state(), nHandler);                  /* L: ... func */
    if (!lua_isfunction(LuaWrapper::get_lua_state(), -1))
    {
        log_error("[LUA ERROR] function refid '%d' does not reference a Lua function", nHandler);
        lua_pop(LuaWrapper::get_lua_state(), 1);
        return false;
    }
    return true;
}

static int execute_service_function(int nHandler, int numArgs) {
    int ret = 0;
    if (push_service_function(nHandler))                                /* L: ... arg1 arg2 ... func */
    {
        if (numArgs > 0)
        {
            lua_insert(LuaWrapper::get_lua_state(), -(numArgs + 1));                        /* L: ... func arg1 arg2 ... */
        }
        ret = exe_function(numArgs);
    }
    lua_settop(LuaWrapper::get_lua_state(), 0);
    return ret;
}

class lua_service : public Service
{
public:
    unsigned int lua_recv_raw_handler;
    unsigned int lua_recv_cmd_handler;
    unsigned int lua_disconnect_handler;

public:
    virtual bool on_session_recv_cmd(Session* s, cmd_msg* msg);
    virtual bool on_session_recv_raw(Session* s, raw_cmd_msg* raw);
    virtual void on_session_disconnect(Session* s, int stype);
};

static void push_proto_message_tolua(Message* message)
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

/*
return format
{
    stype = 1,
	ctype = 2,
	utag  = 3,
	body  = table or string
}
*/
bool lua_service::on_session_recv_cmd(Session* s, struct cmd_msg* msg) {
	tolua_pushuserdata(LuaWrapper::get_lua_state(), (void*)s);
	int index = 1;

	lua_newtable(LuaWrapper::get_lua_state());
	lua_pushinteger(LuaWrapper::get_lua_state(), msg->stype);
	lua_rawseti(LuaWrapper::get_lua_state(), -2, index);          /* table[index] = value, L: table */
	++index;

	lua_pushinteger(LuaWrapper::get_lua_state(), msg->ctype);
	lua_rawseti(LuaWrapper::get_lua_state(), -2, index);          /* table[index] = value, L: table */
	++index;

	lua_pushinteger(LuaWrapper::get_lua_state(), msg->utag);
	lua_rawseti(LuaWrapper::get_lua_state(), -2, index);          /* table[index] = value, L: table */
	++index;

	if (!msg->body) {
		lua_pushnil(LuaWrapper::get_lua_state());
        lua_rawseti(LuaWrapper::get_lua_state(), -2, index);          /* table[index] = value, L: table */
        ++index;
	}
	else {
		if (ProtoManager::proto_type() == PROTO_JSON) {
			lua_pushstring(LuaWrapper::get_lua_state(), (char*) msg->body);
		}
		else { // protobuf
			push_proto_message_tolua((Message*)msg->body);
		}
		lua_rawseti(LuaWrapper::get_lua_state(), -2, index);          /* table[index] = value, L: table */
		++index;
	}

	execute_service_function(this->lua_recv_cmd_handler, 2);
	return true;
}

bool lua_service::on_session_recv_raw(Session* s, raw_cmd_msg* raw) {
    tolua_pushuserdata(LuaWrapper::get_lua_state(), (void*)s);
    tolua_pushuserdata(LuaWrapper::get_lua_state(), (void*)raw);

    execute_service_function(this->lua_recv_raw_handler, 2);
    return true;
}

void lua_service::on_session_disconnect(Session* s, int stype) {
    tolua_pushuserdata(LuaWrapper::get_lua_state(), (void*)s);
    lua_pushinteger(LuaWrapper::get_lua_state(), stype);

    execute_service_function(this->lua_disconnect_handler, 2);
}

static void init_service_function_map(lua_State* L)
{
    lua_pushstring(L, SERVICE_FUNCTION_MAPPING);
    lua_newtable(L);
    lua_rawset(L, LUA_REGISTRYINDEX);

}

static unsigned int s_function_ref_id = 0;
static unsigned int save_service_function(lua_State* L, int lo, int def)
{
    // function at lo
    if (!lua_isfunction(L, lo)) return 0;

    s_function_ref_id++;

    lua_pushstring(L, SERVICE_FUNCTION_MAPPING);
    lua_rawget(L, LUA_REGISTRYINDEX);                           /* stack: fun ... refid_fun */
    lua_pushinteger(L, s_function_ref_id);                      /* stack: fun ... refid_fun refid */
    lua_pushvalue(L, lo);                                       /* stack: fun ... refid_fun refid fun */

    lua_rawset(L, -3);                  /* refid_fun[refid] = fun, stack: fun ... refid_ptr */
    lua_pop(L, 1);                                              /* stack: fun ... */

    return s_function_ref_id;
}

static int lua_register_service(lua_State* tolua_S)
{
    int stype = tolua_tonumber(tolua_S, 1, 0);
    bool ret = false;
    unsigned int lua_recv_cmd_handler;
    unsigned int lua_disconnect_handler;
    lua_service* s = new lua_service();

    if (stype == 0)
        goto lua_failed;

    // table
    if (!lua_istable(tolua_S, 2))
        return 0;

    lua_getfield(tolua_S, 2, "on_session_recv_cmd");
    lua_getfield(tolua_S, 2, "on_session_disconnect");

    lua_recv_cmd_handler = save_service_function(tolua_S, 3, 0);
    lua_disconnect_handler = save_service_function(tolua_S, 4, 0);
    if (lua_recv_cmd_handler == 0 || lua_disconnect_handler == 0) {
        goto lua_failed;
    }

    s->use_raw_cmd = false;
    s->lua_recv_cmd_handler = lua_recv_cmd_handler;
    s->lua_disconnect_handler = lua_disconnect_handler;
    s->lua_recv_raw_handler = 0;
    if (lua_recv_cmd_handler == 0 || lua_disconnect_handler == 0) {
        goto lua_failed;
    }

    ret = ServiceManager::register_service(stype, s);

lua_failed:
    lua_pushboolean(tolua_S, ret ? 1 : 0);
    return 1;
}


static int lua_register_service_with_raw(lua_State* tolua_S)
{
    int stype = tolua_tonumber(tolua_S, 1, 0);
    bool ret = false;
    unsigned int lua_recv_raw_handler;
    unsigned int lua_disconnect_handler;
    lua_service* s = new lua_service();

    if (stype == 0)
        goto lua_failed;

    // table
    if (!lua_istable(tolua_S, 2))
        return 0;

    lua_getfield(tolua_S, 2, "on_session_recv_raw");
    lua_getfield(tolua_S, 2, "on_session_disconnect");

    lua_recv_raw_handler = save_service_function(tolua_S, 3, 0);
    lua_disconnect_handler = save_service_function(tolua_S, 4, 0);

    if (lua_recv_raw_handler == 0 || lua_disconnect_handler == 0) {
        goto lua_failed;
    }

    s->use_raw_cmd = true;
    s->lua_recv_cmd_handler = 0;
    s->lua_disconnect_handler = lua_disconnect_handler;
    s->lua_recv_raw_handler = lua_recv_raw_handler;

    ret = ServiceManager::register_service(stype, s);

lua_failed:
    lua_pushboolean(tolua_S, ret ? 1 : 0);
    return 1;
}

int register_service_export(lua_State* tolua_S)
{
    init_service_function_map(tolua_S);
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S, "Service", 0);
        tolua_beginmodule(tolua_S, "Service");

        // lua format: Service.register(stype, {on_session_recv_cmd = function, on_session_disconnect = function})
        // lua return: true/false
        tolua_function(tolua_S, "register", lua_register_service);
        // lua format: Service.register_with_raw(stype, {on_session_recv_raw = function, on_session_disconnect = function})
        // lua return: true/false
        tolua_function(tolua_S, "register_with_raw", lua_register_service_with_raw);

        tolua_endmodule(tolua_S);
    }

    lua_pop(tolua_S, 1);

    return 1;
}
