local Stype = require("stype")
local Cmd = require("cmd")

local function session_recv_cmd(s, cmd_msg)
    print("stype: " .. cmd_msg[1] .. "\tctype: " .. cmd_msg[2] .. "\tutag: " .. cmd_msg[3])
    -- send to client
    Session.send_msg(s, {
        stype = Stype.Auth,
        ctype = Cmd.eLoginRes,
        utag  = cmd_msg[3],
        body  = {
            status = 1
        }
    })
end

local function session_disconnect(s, stype)

end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
