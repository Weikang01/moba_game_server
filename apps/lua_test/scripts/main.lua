Logger.init("log/", "netbus_log", true, -5)

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}

ProtoManager.init(proto_type.PROTO_BUF)
-- if it's proto_buf, register cmd map
if (ProtoManager.proto_type() == proto_type.PROTO_BUF) then
    local cmd_name_map = require("cmd_name_map")
    if cmd_name_map then
        ProtoManager.register_protobuf_cmd_map(cmd_name_map)
    end
end

-- register services
local chatroom_server = require("chatroom_server")
local ret = Service.register(chatroom_server.stype, chatroom_server.service);

if ret then
	print("register chatroom service success")
else
	print("register chatroom service failed")
end

Netbus.tcp_listen(8023)
Netbus.ws_listen(8043)
Netbus.udp_listen(8063)

