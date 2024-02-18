local Stype = require("stype")
local Cmd = require("cmd")
local game_config = require("game_config")
local g_proto_type = require("proto_type")
local Json = require("json")
local Responses = require("responses")

-- stype -> session
local map_server_session = {}
-- stype -> bool
local map_is_connecting = {}

local servers_config = game_config.servers

local function connect_to_server(stype, ip, port)
    Netbus.tcp_connect(ip, port, function(err, session)
        map_is_connecting[stype] = false
        if err ~= 0 then
            print("connect to server [" ..
                servers_config[stype].desc ..
                "] at " .. servers_config[stype].ip .. ":" .. servers_config[stype].port .. " failed!")
            return
        end
        map_server_session[stype] = session
        print("connected to server [" ..
            servers_config[stype].desc .. "] at " .. servers_config[stype].ip .. ":" .. servers_config[stype].port)
    end)
end

local function check_server_connect()
    for key, value in pairs(servers_config) do
        if map_server_session[value.stype] == nil and map_is_connecting[value.stype] == false then
            map_is_connecting[value.stype] = true
            print("connecting to server [" .. value.desc .. "] at " .. value.ip .. ":" .. value.port)
            connect_to_server(value.stype, value.ip, value.port)
        end
    end
end

local function gateway_service_init()
    for key, value in pairs(servers_config) do
        map_server_session[value.stype] = nil
        map_is_connecting[value.stype] = false
    end

    -- start a scheduler
    Scheduler.schedule(check_server_connect, 0, 5000, -1)
end

-- get client session through temporary ukey
local g_ukey               = 1
local client_sessions_ukey = {}
-- get client session through uid
local client_sessions_uid  = {}

local function is_login_return_cmd(ctype)
    if ctype == Cmd.eGuestLoginRes or
        ctype == Cmd.eUserLoginRes then
        return true
    end
    return false
end

local function send_to_client(server_session, raw_cmd)
    local stype, ctype, utag = RawCmd.read_header(raw_cmd)
    local client_session = nil

    if is_login_return_cmd(ctype) then
        local body = RawCmd.read_body(raw_cmd)
        if g_proto_type == ProtoType.PROTO_JSON then
            body = Json.decode(body)
        end
        client_session = client_sessions_ukey[utag]
        client_sessions_ukey[utag] = nil
        if client_session == nil then
            return
        end

        if body.status ~= Responses.OK then
            RawCmd.set_utag(raw_cmd, 0)
            Session.send_raw_msg(client_session, raw_cmd)
            return
        end

        local uid = body.uinfo.uid
        -- check if there's oldname sessions from the user
        if client_sessions_uid[uid] and client_sessions_uid[uid] ~= client_session then
            local relogin_cmd = {
                stype = Stype.Auth,
                ctype = Cmd.eReloginRes,
                utag  = 0,
                body  = nil
            }
            Session.send_msg(client_sessions_uid[uid], relogin_cmd)
            Session.close(client_sessions_uid[uid]) -- close connection with the old session
        end

        client_sessions_uid[uid] = client_session
        Session.set_uid(client_session, uid)
        body.uinfo.uid = 0
        Session.send_msg(client_session, {
            stype = Stype.Auth,
            ctype = ctype,
            utag  = 0,
            body  = body
        })
    else
        client_session = client_sessions_uid[utag]
        if client_session then
            Session.send_raw_msg(client_session, raw_cmd)
        end
    end
end

local function is_login_request_cmd(ctype)
    if ctype == Cmd.eGuestLoginReq or
        ctype == Cmd.eUserLoginReq then
        return true
    end
    return false
end

local function send_to_server(client_session, raw_cmd)
    local stype, ctype, utag = RawCmd.read_header(raw_cmd)
    local server_session = map_server_session[stype]
    if server_session == nil then -- server error
        Logger.error("stype: " .. stype .. " server error!")
        return
    end

    if is_login_request_cmd(ctype) then -- before login
        utag = Session.get_utag(client_session)
        if utag == 0 then               -- no previous utag
            utag = g_ukey               -- assign an random ukey
            g_ukey = g_ukey + 1
            Session.set_utag(client_session, utag)
        end
        client_sessions_ukey[utag] = client_session
    else                 -- after login
        local uid = Session.get_uid(client_session)
        if uid == 0 then -- this operation needs login first
            return
        end
        -- client_sessions_uid[uid] = client_session
        utag = uid
    end

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

local function session_disconnect(session, stype)
    -- session connected to server is disconnected
    if (Session.as_client(session)) then
        for key, value in pairs(map_server_session) do
            if value == session then
                print("gateway server disconnected with " .. game_config.servers[key].desc)
                map_server_session[key] = nil
                return
            end
        end
        return
    end

    -- session connect to client is disconnected
    -- remove client session from client_sessions_ukey
    local utag = Session.get_utag(session)
    if client_sessions_ukey[utag] ~= nil and client_sessions_ukey[utag] == session then
        client_sessions_ukey[utag] = nil
        Session.set_utag(session, 0)
        -- print("client [" .. utag .. "] removed from client_sessions_ukey")
    end

    -- remove client session from client_sessions_uid
    local uid = Session.get_uid(session)
    if client_sessions_uid[uid] ~= nil and client_sessions_uid[uid] == session then
        client_sessions_uid[uid] = nil
        -- print("client [" .. utag .. "] removed from client_sessions_uid")
    end

    local server_session = map_server_session[stype]
    if server_session == nil then
        return
    end

    -- user lost connection, send this event to other servers
    if uid ~= 0 then
        local userLostConnReq = {
            stype = stype,
            ctype = Cmd.eUserLostConn,
            utag = uid,
            body = nil
        }
        Session.send_msg(server_session, userLostConnReq)
    end
end

gateway_service_init()

local gateway_service = {
    on_session_recv_raw = on_session_recv_raw,
    on_session_disconnect = session_disconnect
}

return gateway_service
