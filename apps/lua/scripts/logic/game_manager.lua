local Stype                = require("stype")
local Cmd                  = require("cmd")
local Responses            = require("responses")
local mysql_auth_center    = require("db.mysql_auth_center")
local redis_center         = require("db.redis_center")
local mysql_moba_game      = require("db.mysql_moba_game")
local redis_game           = require("db.redis_game")
local Player               = require("logic.player")
local Zones                = require("logic.zone")
local State                = require("logic.state")
local MatchManager         = require("logic.match_manager")

local logic_server_players = {} -- k: uid -> v: player
local online_player_num    = 0
local zone_wait_list       = {} -- zone_wait_list[zones.zone1] = {}
local zone_match_list      = {} -- zone_match_list[zones.zone1] = {v: MatchManager}

local function do_load_robot_uinfo(uid)
    mysql_auth_center.get_uinfo_from_uid(uid, function(err, ret)
        if err then
            return
        end

        redis_center.set_uinfo_to_redis(uid, ret)
        print("robot [" .. uid .. "] now added to redis!")
    end)
end

local function do_load_robot_ugame_info()
    mysql_moba_game.load_robot_ugame_info(function(err, ret)
        if err then
            return
        end

        if not ret or #ret <= 0 then
            return
        end

        for _, ugameinfo in ipairs(ret) do
            redis_game.set_ugameinfo(ugameinfo.uid, ugameinfo)
            do_load_robot_uinfo(ugameinfo.uid)
        end
    end)
end

local function load_robots()
    if not mysql_auth_center.is_connected()
        or not mysql_moba_game.is_connected()
        or not redis_center.is_connected()
        or not redis_game.is_connected() then
        Scheduler.once(load_robots, 5000)
    end

    -- load ugame info from mysql_moba_game
    do_load_robot_ugame_info()
end

load_robots()

for zone_name, zone_id in pairs(Zones) do
    zone_match_list[zone_id] = {}
end

local function send_status(session, stype, ctype, uid, status)
    Session.send_msg(session, {
        stype = stype,
        ctype = ctype,
        utag  = uid,
        body  = {
            status = status
        }
    })
end

local function search_inview_match_manager(zone_id)
    local match_list = zone_match_list[zone_id]
    if not match_list then
        zone_match_list[zone_id] = {}
    end

    local i
    local match_manager = nil
    for key, m_manager in pairs(match_list) do
        if m_manager.state == State.inView then
            return m_manager
        end
    end

    local match = MatchManager:new()
    table.insert(match_list, match)
    match:init(zone_id)

    return match
end

local function do_match_players_in_zone(zone_id)
    local wait_list = zone_wait_list[zone_id]
    if not wait_list then
        return
    end

    for uid, player in pairs(wait_list) do
        local match = search_inview_match_manager(zone_id)
        if match then
            if not match:enter_match(player) then
                Logger.error("match system error: player state: " .. player.state .. " match state: " .. match.state)
            end

            wait_list[uid] = nil
        end
    end
end

local function do_match_players_in_zones()
    for zone_name, zone_id in pairs(Zones) do
        do_match_players_in_zone(zone_id)
    end
end

Scheduler.schedule(do_match_players_in_zones, 1000, 5000, -1)

-- {stype, ctype, utag, body}
local function logic_login(session, cmd_msg)
    local uid = cmd_msg[3]
    local stype = cmd_msg[1]
    local p = logic_server_players[uid] -- player object
    if p then                           -- player already exists, just renew the session
        p:set_session(session)
        send_status(session, stype, Cmd.eLogicLoginRes, uid, Responses.OK)
        return
    end

    p = Player:new()
    p:init(uid, session, function(status)
        if status == Responses.OK then
            logic_server_players[uid] = p
            online_player_num = online_player_num + 1
        else
            send_status(session, stype, Cmd.eLogicLoginRes, status)
        end
    end)

    -- other stuff ...

    send_status(session, stype, Cmd.eLogicLoginRes, uid, Responses.OK)
end

-- {stype, ctype, utag, body}
local function on_user_lost_conn(session, cmd_msg)
    local uid = cmd_msg[3]
    local p = logic_server_players[uid]
    if not p then
        return
    end

    if p.zid ~= -1 then
        if zone_wait_list[p.zid][p.uid] then
            zone_wait_list[p.zid][p.uid] = nil
            p.zid = -1
            return
        else
            if p.match_id ~= -1 then
                local c_match = zone_match_list[p.zid][p.match_id]
                c_match:quit_match(p)
            end
        end
    end

    if logic_server_players[uid] then
        logic_server_players[uid] = nil
        online_player_num = online_player_num - 1
    end
end

local function on_gateway_session_connect(session)
    for uid, player in pairs(logic_server_players) do
        player:set_session(session)
    end
end

local function on_gateway_session_disconnect(session)
    for uid, player in pairs(logic_server_players) do
        player:set_session(nil)
    end
end

-- {stype, ctype, utag, body}
local function enter_zone(session, cmd_msg)
    local stype = cmd_msg[1]
    local uid   = cmd_msg[3]
    local p     = logic_server_players[uid]

    if not p then
        send_status(session, stype, Cmd.eEnterZoneRes, uid, Responses.INVALID_OPT)
        return
    end

    local zid = cmd_msg[4].zid
    if zid ~= Zones.zone1 and zid ~= Zones.zone2 then
        send_status(session, stype, Cmd.eEnterZoneRes, uid, Responses.INVALID_PARAMS)
        return
    end

    if not zone_wait_list[zid] then
        zone_wait_list[zid] = {}
    end

    zone_wait_list[zid][uid] = p
    p.zid = zid
    send_status(session, stype, Cmd.eEnterZoneRes, uid, Responses.OK)
end


local game_manager = {
    logic_login = logic_login,
    on_user_lost_conn = on_user_lost_conn,
    on_gateway_session_connect = on_gateway_session_connect,
    on_gateway_session_disconnect = on_gateway_session_disconnect,

    enter_zone = enter_zone,
}

return game_manager
