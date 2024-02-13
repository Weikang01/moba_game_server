logger.init("log/", "netbus_log", true, -5)

local proto_type = {
    PROTO_JSON = 0,
    PROTO_BUF = 1,
}

proto_manager.init(proto_type.PROTO_BUF)
-- if it's proto_buf, register cmd map
if (proto_manager.proto_type() == proto_type.PROTO_BUF) then
    local cmd_name_map = require("cmd_name_map")
    if cmd_name_map then
        proto_manager.register_protobuf_cmd_map(cmd_name_map)
    end
end

-- register services

local echo_server = require("echo_server")
service.register(echo_server.stype, echo_server.service);

netbus.tcp_listen(8023)
netbus.ws_listen(8043)
netbus.udp_listen(8063)
