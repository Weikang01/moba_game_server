Logger.init("log/logic/", "logic", true, -5)
local g_proto_type = require("proto_type")

-- init logic center db
-- require("db/mysql_logic_server")

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

Netbus.tcp_listen(servers[Stype.Logic].port)
Netbus.udp_listen(game_config.logic_udp.port)
print("Logic server starts at " .. servers[Stype.Logic].port .. " and udp listening on: " .. game_config.logic_udp.port)

local logic_service = require("logic.logic_service")
local ret = Service.register(Stype.Logic, logic_service)

if ret then
    print("register logic service[" .. Stype.Logic .. "] success!")
else
    print("register logic service[" .. Stype.Logic .. "] failed!")
end
