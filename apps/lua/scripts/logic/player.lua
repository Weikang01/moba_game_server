local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_moba_game = require("db.mysql_moba_game")
local redis_game = require("db.redis_game")


local player = {}

function player:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end

function player:init(uid, session, ret_handler)
    self.uid = uid
    self.session = session

    -- read player's data from databases
    mysql_moba_game.get_ugame_info(uid, function(err, ugame_info)
        if err then
            if ret_handler then
                ret_handler(Responses.SYSTEM_ERR)
            end
        end

        self.ugame_info = ugame_info
        if ret_handler then
            ret_handler(Responses.OK)
        end
    end)
end

function player:set_session(session)
    self.session = session
end

return player
