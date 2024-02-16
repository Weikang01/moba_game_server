local Stype = require("stype")
local Cmd = require("cmd")
local Json = require("json")

local function session_recv_cmd(s, cmd_msg)
    print("stype: " ..
        cmd_msg[1] .. "\tctype: " .. cmd_msg[2] .. "\tutag: " .. cmd_msg[3] .. "\tbody: " .. cmd_msg[4].guest_key)
end

local function session_disconnect(s, stype)
    print("auth server disconnected!")
end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
