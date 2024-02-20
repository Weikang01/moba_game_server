local player = require("logic.player")

local robot_player = player:new()

function robot_player:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end
