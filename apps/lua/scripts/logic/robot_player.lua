local State = require("logic.state")
local Player = require("logic.player")

local robot_player = Player:new()

function robot_player:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end

function robot_player:init(uid, ugame_info, uinfo)
    Player.init(self, uid, nil, nil, true)
    self.ugame_info = ugame_info
    self.uinfo      = uinfo
end

function robot_player:set_ugameinfo(ugame_info)
    self.ugame_info = ugame_info
end

function robot_player:set_uinfo(uinfo)
    self.uinfo = uinfo
end

function robot_player:quit_match()
    self.match_id = -1
    self.state    = State.inView
end

return robot_player
