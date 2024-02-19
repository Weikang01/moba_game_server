Logger.init("log/system/", "system", true, -5)
local g_proto_type = require("proto_type")

-- init auth center db
require("db.mysql_moba_game")

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

Netbus.tcp_listen(servers[Stype.System].port)
print("System server starts at " .. servers[Stype.System].port)

local system_service = require("system.system_service")
local ret = Service.register(Stype.System, system_service)

if ret then
    print("register system service[" .. Stype.System .. "] success!")
else
    print("register system service[" .. Stype.System .. "] failed!")
end
