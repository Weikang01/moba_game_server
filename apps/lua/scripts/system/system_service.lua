local Cmd = require("cmd")
local GetUGameInfo = require("system.get_ugame_info")

local system_service_handlers = {}
system_service_handlers[Cmd.eGetUGameInfoReq] = GetUGameInfo.get_ugame_info

local function session_recv_cmd(s, cmd_msg)
    if system_service_handlers[cmd_msg[2]] then
        system_service_handlers[cmd_msg[2]](s, cmd_msg)
    end
end

local function session_disconnect(s, stype)
    print("system server disconnected!")
end


local system_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return system_service
