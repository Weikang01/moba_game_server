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
-- local LOGIC_DELTA_TIME        = 66; -- frame event every 66ms, appx. 15 fps
local LOGIC_DELTA_TIME        = 25; -- frame event every 25ms, appx. 40 fps

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
    self.frameid        = 1

    -- get inview player list
    self.inview_players = {} -- uid to player
    self.teams          = {} -- team_id to list_of_players

    self.match_frames   = {} -- all the frames since game starts
    self.next_frame_opt = {} -- current frame operation
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
    player.sync_frameid = 0
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
            if self.teams[player.team_id] == nil then
                self.teams[player.team_id] = {}
            end
            self.teams[player.team_id][player.seat_id] = player
            break
        end
    end

    -- tell player that they are in waiting room
    player:send_msg(Stype.Logic, Cmd.eEnterMatch, {
        zid         = player.zid,
        matchid     = player.match_id,
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

function MatchManager:send_unsync_frames(player)
    if player.is_robot then
        return
    end

    local opt_frames = {}

    for i = player.sync_frameid + 1, #self.match_frames do
        table.insert(opt_frames, self.match_frames[i])
    end

    -- print("opt_frames.len = " .. tostring(#opt_frames))
    local body = {
        frameid = self.frameid,
        unsync_frames = opt_frames
    }

    player:send_udp_msg(Stype.Logic, Cmd.eLogicFrame, body)
end

function MatchManager:on_frame_sync()
    table.insert(self.match_frames, self.next_frame_opt)

    for _, player in pairs(self.inview_players) do
        self:send_unsync_frames(player)
    end

    self.frameid = self.frameid + 1
    self.next_frame_opt = { frameid = self.frameid, opts = {} }
end

function MatchManager:on_start_playing()
    self.state = State.Playing
    self:update_players_state(State.Playing)

    self.frameid        = 1

    self.match_frames   = {}
    self.next_frame_opt = {
        frameid = self.frameid,
        opts = {}
    }

    Scheduler.schedule(function()
        self:on_frame_sync()
    end, 0, LOGIC_DELTA_TIME, -1)
end

function MatchManager:game_start()
    self.match_frames   = {}
    self.next_frame_opt = {}

    local characters    = {}
    for key, player in pairs(self.inview_players) do
        table.insert(characters, {
            teamid      = player.team_id,
            seatid      = player.seat_id,
            characterid = player.character_id
        })
    end

    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eGameStart, {
        characters = characters
    }, nil)

    self.state = State.Start
    self:update_players_state(State.Start)

    -- after 5 sec, starts the first frame event, then invoke a frame event every 50ms (20 fps)
    self.frame_timer = Scheduler.once(function()
        self:on_start_playing()
    end, 2000)
end

function MatchManager:on_next_frame_event(next_frame_opts)
    local player = self.inview_players[next_frame_opts.seatid]
    if not player then
        return
    end

    if player.sync_frameid < next_frame_opts.frameid - 1 then
        player.sync_frameid = next_frame_opts.frameid - 1
    end

    if next_frame_opts.frameid ~= self.next_frame_opt.frameid then
        return
    end

    for _, value in ipairs(next_frame_opts.opts) do
        table.insert(self.next_frame_opt.opts, value)
    end

    -- print("length of self.next_frame_opt: " .. tostring(#self.next_frame_opt))
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

function MatchManager:game_finish(winner_teamid)
    for uid, player in pairs(self.inview_players) do
        player.state = State.Checkout
    end

    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eGameFinishedRes, {
        winner_teamid = winner_teamid
    }, nil)

    self.state = State.Checkout
end

return MatchManager
