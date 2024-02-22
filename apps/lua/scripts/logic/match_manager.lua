local Stype                   = require("stype")
local Cmd                     = require("cmd")
local Responses               = require("responses")
local mysql_moba_game         = require("db.mysql_moba_game")
local redis_game              = require("db.redis_game")
local Player                  = require("logic.player")
local Zones                   = require("logic.zone")
local State                   = require("logic.state")

local MatchManager            = {}
local g_match_id              = 1
local NR_PLAYERS_IN_EACH_TEAM = 2
local NR_TEAMS                = 2
local NR_CHARACTERS           = 5

function MatchManager:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end

function MatchManager:init(zid)
    self.zid            = zid
    self.match_id       = g_match_id
    g_match_id          = g_match_id + 1
    self.state          = State.inView
    self.human_count    = 0
    self.frameid        = 0

    -- get inview player list
    self.inview_players = {} -- uid to player
    self.teams          = {} -- team_id to list_of_players
end

function MatchManager:broadcast_cmd_inview_players(stype, ctype, body, excluded_player)
    for _, player in pairs(self.inview_players) do
        if excluded_player == nil or player ~= excluded_player then
            player:send_msg(stype, ctype, body)
        end
    end
end

function MatchManager:update_players_state(state)
    for index, value in pairs(self.inview_players) do
        self.inview_players[index].state = state
    end
end

function MatchManager:enter_match(player)
    if self.state ~= State.inView or player.state ~= State.inView then
        return false
    end

    if self.inview_players[player.seat_id] == player then
        return true
    end

    if not player.is_robot then
        self.human_count = self.human_count + 1
    end

    player.match_id = self.match_id
    local other_users = {}
    for _, other in pairs(self.inview_players) do
        table.insert(other_users, other:get_uinfo())
    end

    -- add player
    for i = 1, NR_PLAYERS_IN_EACH_TEAM * NR_TEAMS do
        if self.inview_players[i] == nil then
            self.inview_players[i] = player
            player.seat_id = i
            player.team_id = ((i - 1) % NR_TEAMS) + 1
            break
        end
    end

    -- tell player that they are in waiting room
    player:send_msg(Stype.Logic, Cmd.eEnterMatch, {
        zid         = player.zid,
        matchid     = player.matchid,
        seatid      = player.seat_id,
        teamid      = player.team_id,
        other_uinfo = other_users
    })

    -- broadcast_cmd_inview_players
    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eOnOtherEnteredMatch, player:get_uinfo(), player)

    -- check if room is full
    if #self.inview_players >= NR_PLAYERS_IN_EACH_TEAM * NR_TEAMS then
        self.state = State.Ready

        self:update_players_state(State.Ready)

        -- randomly assign a character id to each player between [1, NR_CHARACTERS]
        for key, c_player in pairs(self.inview_players) do
            c_player.character_id = math.random(1, NR_CHARACTERS)
        end

        self:game_start()
    end

    return true
end

function MatchManager:on_frame_sync()
    self.frameid = self.frameid + 1
    for _, player in pairs(self.inview_players) do
        player:send_udp_msg(Stype.Logic, Cmd.eLogicFrame, {
            frameid = self.frameid
        })
    end
end

function MatchManager:on_start_playing()
    self.state = State.Playing
    self:update_players_state(State.Playing)
    Scheduler.schedule(function()
        self:on_frame_sync()
    end, 0, 50, -1)
end

function MatchManager:game_start()
    local characters = {}
    for key, player in pairs(self.inview_players) do
        table.insert(characters, {
            seatid      = player.seat_id,
            characterid = player.character_id
        })
    end

    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eGameStart, {
        characters = characters
    })

    self.state = State.Start
    self.frameid = 0
    self:update_players_state(State.Start)

    -- after 5 sec, starts the first frame event, then invoke a frame event every 50ms (20 fps)
    self.frame_timer = Scheduler.once(function()
        self:on_start_playing()
    end, 5000)
end

function MatchManager:quit_match(player)
    player:send_msg(Stype.Logic, Cmd.eQuitMatchRes, {
        status = Responses.OK
    })

    if self.inview_players then
        self.inview_players[player.seat_id] = nil
    end

    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eOnOtherQuittedMatch, {
        seatid = player.seat_id
    }, player)

    player:quit_match()
    if not player.is_robot then
        self.human_count = self.human_count - 1
        if self.human_count == 0 then
            for robot_seatid, robot in pairs(self.inview_players) do
                robot:quit_match()
                self.inview_players[robot_seatid] = nil
            end
            return true
        end
    end

    return false
end

return MatchManager
