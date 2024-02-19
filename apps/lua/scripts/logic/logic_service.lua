local Cmd = require("cmd")
local GameManager = require("logic.game_manager")
-- local

local logic_service_handlers = {}
logic_service_handlers[Cmd.eLogicLoginReq] = GameManager.logic_login

local function session_recv_cmd(s, cmd_msg)
    if logic_service_handlers[cmd_msg[2]] then
        logic_service_handlers[cmd_msg[2]](s, cmd_msg)
    end
end

local function session_disconnect(s, stype)
    print("logic server disconnected!")
end


local logic_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return logic_service
