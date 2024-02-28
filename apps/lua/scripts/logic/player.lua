local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_moba_game = require("db.mysql_moba_game")
local redis_center = require("db.redis_center")
local redis_game = require("db.redis_game")
local State = require("logic.state")


local player = {}

function player:new(instance)
    instance = instance or {}
    setmetatable(instance, { __index = self });
    return instance
end

function player:init(uid, session, ret_handler, is_robot)
    self.uid          = uid
    self.session      = session
    self.zid          = -1 -- current zone of player
    self.match_id     = -1 -- id of the match
    self.seat_id      = -1
    self.team_id      = -1 -- player's team within match
    self.character_id = -1 -- which character player is using
    self.state        = State.inView
    self.is_robot     = is_robot or false
    self.client_ip    = nil -- player's udp ip
    self.client_port  = 0   -- player's udp port
    self.sync_frameid = 0   -- current synchronized frameid of clients

    if self.is_robot then
        return
    end

    -- read player's data from databases
    mysql_moba_game.get_ugame_info(uid, function(err, ugame_info)
        if err then
            if ret_handler then
                ret_handler(Responses.SYSTEM_ERR)
            end
            return
        end

        self.ugame_info = ugame_info

        redis_center.get_uinfo_to_redis(uid, function(err, uinfo)
            if err then
                if ret_handler then
                    ret_handler(Responses.SYSTEM_ERR)
                end
                return
            end

            self.uinfo = uinfo
            if ret_handler then
                ret_handler(Responses.OK)
            end
        end)
    end)
end

function player:set_session(session)
    self.session = session
end

function player:send_msg(stype, ctype, body)
    if not self.session or self.is_robot then
        return
    end

    Session.send_msg(self.session, {
        stype = stype,
        ctype = ctype,
        utag  = self.uid,
        body  = body
    })
end

function player:get_uinfo()
    return {
        unick      = self.uinfo.unick,
        usex       = self.uinfo.usex,
        usysavatar = self.uinfo.usysavatar,
        seatid     = self.seat_id,
        teamid     = self.team_id,
    }
end

function player:quit_match()
    self.zid      = -1
    self.match_id = -1
    self.seat_id  = -1
    self.team_id  = -1
    self.state    = State.inView
end

function player:set_udp_address(ip, port)
    self.client_ip   = ip
    self.client_port = port
end

function player:send_udp_msg(stype, ctype, body)
    if not self.session or self.is_robot then -- if tcp connection lost, udp do not need to send anymore
        return
    end

    if not self.client_ip or self.client_port == 0 then
        return
    end

    Session.udp_send_msg(self.client_ip, self.client_port, {
        stype = stype,
        ctype = ctype,
        utag  = 0,
        body  = body
    })
end

return player
