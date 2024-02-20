local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_moba_game = require("db.mysql_moba_game")
local redis_game = require("db.redis_game")
local Player = require("logic.player")
local Zones = require("logic.zone")
local State = require("logic.state")

local MatchManager = {}
local g_match_id = 1
local PLAYER_NUM = 3

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

    -- get inview player list
    self.inview_players = {} -- uid to player
    self.t1_players     = {} -- uid to player
    self.t2_players     = {} -- uid to player
end

function MatchManager:broadcast_cmd_inview_players(stype, ctype, body, excluded_player)
    for _, player in ipairs(self.inview_players) do
        if player ~= excluded_player then
            player:send_msg(stype, ctype, body)
        end
    end
end

function MatchManager:enter_match(player)
    if self.state ~= State.inView or player.state ~= State.inView then
        return false
    end

    player.match_id = self.match_id
    local other_users = {}
    for _, other in ipairs(self.inview_players) do
        table.insert(other_users, other:get_uinfo())
    end

    -- tell player that they are in waiting room
    player:send_msg(Stype.Logic, Cmd.eEnterMatch, {
        zid         = self.zid,
        matchid     = self.matchid,
        other_uinfo = other_users,
    })

    -- add player
    self.inview_players[player.uid] = player

    -- broadcast_cmd_inview_players
    self:broadcast_cmd_inview_players(Stype.Logic, Cmd.eOnOtherEnteredMatch, player:get_uinfo(), player)

    -- check if room is full
    if #self.inview_players >= PLAYER_NUM * 2 then
        self.state = State.Ready
        for index, value in ipairs(self.inview_players) do
            self.inview_players[index].state = State.Ready
        end
    end

    return true
end

function MatchManager:quit_match(player)
    if self.inview_players then
        self.inview_players[player.uid] = nil
    end
end

return MatchManager
