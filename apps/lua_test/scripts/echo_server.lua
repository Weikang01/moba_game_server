local function echo_recv_cmd(s, cmd_msg)
    -- print(cmd_msg[1]) -- stype
    -- print(cmd_msg[2]) -- ctype
    -- print(cmd_msg[3]) -- utag
    local body = cmd_msg[4]
    print(body.username)
    print(body.age)
    print(body.email)

    -- send to client
    session.send_msg(s, {
        stype = cmd_msg[1],
        ctype = 2,
        utag  = 0,
        body  = {
            status = 200
        }
    })
end

local function echo_session_disconnect(s)
    local ip, port = session.get_address(s)
    print("session_disconnect: " .. ip .. ":" .. port)
end

local echo_service = {
    on_session_recv_cmd = echo_recv_cmd,
    on_session_disconnect = echo_session_disconnect
}

local echo_server = {
    stype = 1,
    service = echo_service
}

return echo_server
