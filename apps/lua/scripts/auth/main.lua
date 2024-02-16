Logger.init("log/auth/", "auth", true, -5)
local g_proto_type = require("proto_type")

-- init auth center db
require("db/mysql_auth_center")

ProtoManager.init(g_proto_type)
-- if it's proto_buf, register cmd map
if (ProtoManager.proto_type() == g_proto_type) then
    local cmd_name_map = require("cmd_name_map")
    if cmd_name_map then
        ProtoManager.register_protobuf_cmd_map(cmd_name_map)
    end
end

local game_config = require("game_config")
local servers = game_config.servers
local Stype = require("stype")

Netbus.tcp_listen(servers[Stype.Auth].port)
print("Auth server starts at " .. servers[Stype.Auth].port)

local auth_service = require("auth/auth_service")
local ret = Service.register(Stype.Auth, auth_service)

if ret then
    print("register auth service[" .. Stype.Auth .. "] success!")
else
    print("register auth service[" .. Stype.Auth .. "] failed!")
end
