Logger.init("log/gateway/", "gateway", true, -5)
local g_proto_type = require("proto_type")


ProtoManager.init(g_proto_type)
-- if it's proto_buf, register cmd map
if (ProtoManager.proto_type() == g_proto_type) then
    local cmd_name_map = require("cmd_name_map")
    if cmd_name_map then
        ProtoManager.register_protobuf_cmd_map(cmd_name_map)
    end
end

local game_config = require("game_config")

Netbus.tcp_listen(game_config.gateway_tcp_port)
Netbus.ws_listen(game_config.gateway_ws_port)

print("TCP Server starts at " .. game_config.gateway_tcp_port)
print("WS Server starts at " .. game_config.gateway_ws_port)


local servers = game_config.servers
local gateway_service = require("gateway/gateway_service")
for k, v in pairs(servers) do
    local ret = Service.register_with_raw(v.stype, gateway_service)
    if ret then
        print("gateway service [" .. v.stype .. "] register success!")
    else
        print("gateway service [" .. v.stype .. "] register failed!")
    end
end
