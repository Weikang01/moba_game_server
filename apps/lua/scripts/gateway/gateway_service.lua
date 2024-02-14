--[[
cmd_raw = {
    stype = 1,
    ctype = 2,
    utag  = 3,
    body  = raw
}
]]
local function on_session_recv_raw(s, cmd_raw)

end

local function session_disconnect(s)

end


local gateway_service = {
    on_session_recv_raw = on_session_recv_raw,
    on_session_disconnect = session_disconnect
}

return gateway_service
