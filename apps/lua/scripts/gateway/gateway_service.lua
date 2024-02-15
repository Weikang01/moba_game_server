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

-- get client session through temporary ukey
local g_ukey               = 1
local client_sessions_ukey = {}
-- get client session through uid
local client_sessions_uid  = {}

local function send_to_client(server_session, raw_cmd)
    local stype, ctype, utag = RawCmd.read_header(raw_cmd)
    local client_session = nil
    if client_sessions_uid[utag] ~= nil then
        client_session = client_sessions_uid[utag]
    elseif client_sessions_ukey[utag] ~= nil then
        client_session = client_sessions_ukey[utag]
    else
        Logger.error("unknown utag [" .. utag .. "]")
        return
    end
    Session.send_raw_msg(client_session, raw_cmd)
end


local function send_to_server(client_session, raw_cmd)
    local stype, ctype, utag = RawCmd.read_header(raw_cmd)
    local server_session = map_server_session[stype]
    if server_session == nil then -- server error
        return
    end

    local uid = Session.get_uid(client_session)
    if uid == 0 then      -- before login
        utag = Session.get_utag(client_session)
        if utag == 0 then -- no previous utag
            utag = g_ukey -- assign an random ukey
            g_ukey = g_ukey + 1
            client_sessions_ukey[utag] = client_session
            Session.set_utag(client_session, utag)
        end
    else -- after login
        utag = uid
        client_sessions_uid[utag] = client_session
    end

    -- set utag and send to server
    RawCmd.set_utag(raw_cmd, utag)
    Session.send_raw_msg(server_session, raw_cmd)
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
    if (Session.as_client(s)) then -- sent from other servers to client
        send_to_client(s, cmd_raw)
    else
        send_to_server(s, cmd_raw) -- sent from client dispatch to servers
    end
end

local function session_disconnect(s, stype)
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
    -- remove client session from client_sessions_ukey
    local utag = Session.get_utag(s)
    if client_sessions_ukey[utag] ~= nil then
        client_sessions_ukey[utag] = nil
        Session.set_utag(s, 0)
        print("client [" .. utag .. "] removed from client_sessions_ukey")
    end
    -- remove client session from client_sessions_uid
    local uid = Session.get_uid(s)
    if client_sessions_uid[uid] ~= nil then
        client_sessions_uid[uid] = nil
        print("client [" .. utag .. "] removed from client_sessions_uid")
    end

    -- user dropped connection, send this event to other servers
    if uid ~= 0 and servers[stype] ~= nil then
    end
end

gateway_service_init()

local gateway_service = {
    on_session_recv_raw = on_session_recv_raw,
    on_session_disconnect = session_disconnect
}

return gateway_service
