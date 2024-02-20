local Cmd = require("cmd")
local GameManager = require("logic.game_manager")
-- local

local logic_service_handlers = {}
logic_service_handlers[Cmd.eLogicLoginReq] = GameManager.logic_login
logic_service_handlers[Cmd.eUserLostConn] = GameManager.on_user_lost_conn
logic_service_handlers[Cmd.eEnterZoneReq] = GameManager.enter_zone


local function session_recv_cmd(s, cmd_msg)
    if logic_service_handlers[cmd_msg[2]] then
        logic_service_handlers[cmd_msg[2]](s, cmd_msg)
    end
end

local function on_gateway_session_connect(s, stype)
    print("logic server connected to gateway!")
    GameManager.on_gateway_session_connect(s)
end

local function on_gateway_session_disconnect(s, stype)
    print("logic server disconnected from gateway!")
    GameManager.on_gateway_session_disconnect(s)
end


local logic_service = {
    on_session_connect = on_gateway_session_connect,
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = on_gateway_session_disconnect
}

return logic_service
