local game_config = require("game_config")


-- stype -> session
local map_server_session = {}
-- stype -> bool
local map_is_connecting = {}

local servers = game_config.servers

local function connect_to_server(stype, ip, port)
    Netbus.tcp_connect(ip, port, function(err, session)
        map_is_connecting[stype] = false
        if err ~= 0 then
            print("connect to server [" ..
                servers[stype].desc .. "] at " .. servers[stype].ip .. ":" .. servers[stype].port .. " failed!")
            return
        end
        map_server_session[stype] = session
        print("connected to server [" ..
            servers[stype].desc .. "] at " .. servers[stype].ip .. ":" .. servers[stype].port)
    end)
end

local function check_server_connect()
    for key, value in pairs(game_config.servers) do
        if map_server_session[value.stype] == nil and map_is_connecting[value.stype] == false then
            map_is_connecting[value.stype] = true
            print("connecting to server [" .. value.desc .. "] at " .. value.ip .. ":" .. value.port)
            connect_to_server(value.stype, value.ip, value.port)
        end
    end
end

local function gateway_service_init()
    for key, value in pairs(game_config.servers) do
        map_server_session[value.stype] = nil
        map_is_connecting[value.stype] = false
    end

    -- start a scheduler
    Scheduler.schedule(check_server_connect, 1000, 5000, -1)
end

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
    -- session connected to server is disconnected
    if (Session.as_client(s)) then
        for key, value in pairs(map_server_session) do
            if value == s then
                print("gateway server disconnected with " .. game_config.servers[key].desc)
                map_server_session[key] = nil
                return
            end
        end
        return
    end

    -- session connect to client is disconnected
end

gateway_service_init()

local gateway_service = {
    on_session_recv_raw = on_session_recv_raw,
    on_session_disconnect = session_disconnect
}

return gateway_service
