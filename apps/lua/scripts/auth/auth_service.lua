local function session_recv_cmd(s, cmd_msg)

end

local function session_disconnect(s)

end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
