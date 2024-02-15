local function session_recv_cmd(s, cmd_msg)
    print("stype: " .. cmd_msg[1] .. "\tctype: " .. cmd_msg[2] .. "\tutag: " .. cmd_msg[3])
end

local function session_disconnect(s)

end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
