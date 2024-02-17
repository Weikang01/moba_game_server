local Cmd = require("cmd")
local Guest = require("auth/guest")

local auth_service_handlers = {}
auth_service_handlers[Cmd.eGuestLoginReq] = Guest.login


local function session_recv_cmd(s, cmd_msg)
    if auth_service_handlers[cmd_msg[2]] then
        auth_service_handlers[cmd_msg[2]](s, cmd_msg)
    end
end

local function session_disconnect(s, stype)
    print("auth server disconnected!")
end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
