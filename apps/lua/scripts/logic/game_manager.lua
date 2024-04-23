local Stype                = require("stype")
local Cmd                  = require("cmd")
local Responses            = require("responses")
local mysql_auth_center    = require("db.mysql_auth_center")
local redis_center         = require("db.redis_center")
local mysql_moba_game      = require("db.mysql_moba_game")
local redis_game           = require("db.redis_game")
local Player               = require("logic.player")
local RobotPlayer          = require("logic.robot_player")
local Zones                = require("logic.zone")
local State                = require("logic.state")
local MatchManager         = require("logic.match_manager")
local utils                = require("utils")

local logic_server_players = {} -- k: uid -> v: player
local online_player_num    = 0
local zone_list            = {}
local zone_wait_list       = {} -- zone_wait_list[zones.zone1] = {}
local zone_match_list      = {} -- zone_match_list[zones.zone1] = {v: MatchManager}
local zone_robot_list      = {}

for _, zone_id in pairs(Zones) do -- initialize zone_robot_list
    table.insert(zone_list, zone_id)
end

for _, zone_id in ipairs(zone_list) do
    zone_wait_list[zone_id] = {}
end

for _, zone_id in ipairs(zone_list) do
    zone_match_list[zone_id] = {}
end

for _, zone_id in ipairs(zone_list) do -- initialize zone_robot_list
    zone_robot_list[zone_id] = {}
end

local function do_load_robot_ugame_info()
    mysql_moba_game.load_robot_ugame_info(function(err, gameinfos)
        if err then
            return
        end

        if not gameinfos or #gameinfos <= 0 then
            return
        end

        local index = 0
        for _, ugameinfo in ipairs(gameinfos) do
            local robot = RobotPlayer:new()
            robot:init(ugameinfo.uid, ugameinfo, nil)
            redis_game.set_ugameinfo(ugameinfo.uid, ugameinfo)

            mysql_auth_center.get_uinfo_from_uid(robot.uid, function(erro, ret)
                if erro then
                    return
                end

                redis_center.set_uinfo_to_redis(robot.uid, ret)
                robot.zid = index % #zone_list + 1
                robot.uinfo = ret
                zone_robot_list[zone_list[index % #zone_list + 1]][ugameinfo.uid] = robot
                index = index + 1
            end)
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

Scheduler.once(load_robots, 2000)

local function send_status(session, stype, ctype, uid, status)
    if session == nil then
        return
    end
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

    for match_id, m_manager in pairs(match_list) do
        -- print("found the match [" .. match_id .. "]!")
        if m_manager.state == State.inView then
            return m_manager
        end
    end

    local match = MatchManager:new()
    match:init(zone_id)
    zone_match_list[zone_id][match.match_id] = match

    return match
end

local function do_match_players_in_zone(zone_id)
    if not zone_wait_list[zone_id] then
        return
    end

    for uid, player in pairs(zone_wait_list[zone_id]) do
        local match = search_inview_match_manager(zone_id)
        if match then
            if not match:enter_match(player) then
                Logger.error("match system error: player state: " .. player.state .. " match state: " .. match.state)
            end

            zone_wait_list[zone_id][uid] = nil
        end
    end
end

local function do_match_players_in_zones()
    for _, zone_id in ipairs(zone_list) do
        do_match_players_in_zone(zone_id)
    end
end

Scheduler.schedule(do_match_players_in_zones, 1000, 2000, -1)

local function find_idle_robot(zone_id)
    for uid, robot in pairs(zone_robot_list[zone_id]) do
        if robot.match_id == -1 then
            return robot
        end
    end
    return nil
end

local function do_push_robot_to_match()
    for zone_id, match_list in pairs(zone_match_list) do
        for match_id, match in pairs(match_list) do
            if match.state == State.inView then
                local robot = find_idle_robot(zone_id)
                if robot then
                    robot.match_id = match_id
                    match:enter_match(robot)
                    -- test
                    -- Scheduler.once(function()
                    --     match:quit_match(robot)
                    -- end, 3000)
                    -- test end
                end
            end
        end
    end
end

-- Scheduler.schedule(do_push_robot_to_match, 1000, 400, -1)

-- {stype, ctype, utag, body}
local function logic_login(session, cmd_msg)
    local stype = cmd_msg[1]
    local uid   = cmd_msg[3]
    local body  = cmd_msg[4]

    -- print(body.ip, body.udp_port)

    local p     = logic_server_players[uid] -- player object
    if p then                               -- player already exists, just renew the session
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
    p:set_udp_address(body.udp_ip, body.udp_port)

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

    p:set_session(nil) -- prevent UDP calls
    p:set_udp_address(nil, 0)

    if p.zid ~= -1 then
        zone_wait_list[p.zid][p.uid] = nil

        if p.match_id ~= -1 then
            local c_match = zone_match_list[p.zid][p.match_id]
            if c_match == nil then
                print("What?? c_match==nil?? p.zid:" .. p.zid .. "\tp.match_id: " .. p.match_id)
            end
            local p_zid = p.zid
            local p_match_id = p.match_id
            if c_match:quit_match(p) then
                zone_match_list[p_zid][p_match_id] = nil
            end
        end
    end

    -- remove player from list
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

    -- print(
    --     " uid: " .. tostring(p.uid) ..
    --     " session: " .. tostring(p.session) ..
    --     " zid: " .. tostring(p.zid) ..
    --     " match_id: " .. tostring(p.match_id) ..
    --     " seat_id: " .. tostring(p.seat_id) ..
    --     " state: " .. tostring(p.state) ..
    --     " is_robot: " .. tostring(p.is_robot)
    -- )

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

    if p.zid == zid then
        send_status(session, stype, Cmd.eEnterZoneRes, uid, Responses.OK)
        return
    elseif zone_wait_list[p.zid] and zone_wait_list[p.zid][p.uid] ~= nil then
        zone_wait_list[p.zid][p.uid] = nil
    elseif p.match_id ~= -1 and zone_match_list[p.zid] and zone_match_list[p.zid][p.match_id] then
        local prev_match = zone_match_list[p.zid][p.match_id]
        prev_match:quit_match(p)
    end

    p.match_id = -1
    p.zid = zid
    p.state = State.inView

    zone_wait_list[zid][uid] = p
    send_status(session, stype, Cmd.eEnterZoneRes, uid, Responses.OK)
end

-- {stype, ctype, utag, body}
local function on_quit_match(session, cmd_msg)
    local stype  = cmd_msg[1]
    local uid    = cmd_msg[3]
    local player = logic_server_players[uid]
    if not player then
        send_status(session, stype, Cmd.eQuitMatchRes, uid, Responses.INVALID_OPT)
        return
    end

    if (player.state ~= State.inView and player.state ~= State.Checkout)
        or player.zid == -1
        or player.match_id == -1
        or player.seat_id == -1 then
        send_status(session, stype, Cmd.eQuitMatchRes, uid, Responses.INVALID_OPT)
        return
    end

    local match = zone_match_list[player.zid][player.match_id]

    if not match or (match.state ~= State.inView and match.state ~= State.Checkout) then
        send_status(session, stype, Cmd.eQuitMatchRes, uid, Responses.INVALID_OPT)
        return
    end
    local zid = player.zid
    local match_id = player.match_id

    if match:quit_match(player) then
        zone_match_list[zid][match_id] = nil
    end
end

-- {stype, ctype, utag, body}
local function on_next_frame_event(session, cmd_msg)
    local stype = cmd_msg[1]
    local uid   = cmd_msg[3]
    local body  = cmd_msg[4]
    --[[
    int32 frameid                    = 1;
	int32 zid                        = 2;
	int32 matchid                    = 3;
	repeated OptionEvent opts        = 4;
    ]]
    local match = zone_match_list[body.zid][body.matchid]

    if not match or match.state ~= State.Playing then
        return
    end

    match:on_next_frame_event(body)
end

-- {stype, ctype, utag, body}
local function on_game_finished(session, cmd_msg)
    local stype  = cmd_msg[1]
    local uid    = cmd_msg[3]
    local player = logic_server_players[uid]
    if not player or player.state ~= State.Playing then
        -- send_status(session, stype, Cmd.eGameFinishedRes, uid, Responses.INVALID_OPT)
        return
    end

    local match = zone_match_list[player.zid][player.match_id]
    if not match then
        return
    end

    match:game_finish(player.team_id)
end

local game_manager = {
    logic_login                   = logic_login,
    on_user_lost_conn             = on_user_lost_conn,
    on_gateway_session_connect    = on_gateway_session_connect,
    on_gateway_session_disconnect = on_gateway_session_disconnect,
    enter_zone                    = enter_zone,
    on_quit_match                 = on_quit_match,
    on_next_frame_event           = on_next_frame_event,
    on_game_finished              = on_game_finished,
}

return game_manager
