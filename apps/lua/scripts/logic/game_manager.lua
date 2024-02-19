local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_moba_game = require("db.mysql_moba_game")
local redis_game = require("db.redis_game")
local Player = require("logic.player")

local logic_server_players = {} -- k: uid -> v: player
local online_player_num = 0

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


-- {stype, ctype, utag, body}
local function logic_login(session, cmd_msg)
    local uid = cmd_msg[3]
    local p = logic_server_players[uid] -- player object
    if p then                           -- player already exists, just renew the session
        p:set_session(session)
        send_status(session, Stype.Logic, Cmd.eLogicLoginRes, uid, Responses.OK)
        return
    end

    p = Player:new()
    p:init(uid, session, function(status)
        if status == Responses.OK then
            logic_server_players[uid] = p
            online_player_num = online_player_num + 1
        else
            send_status(session, Stype.Logic, Cmd.eLogicLoginRes, status)
        end
    end)

    -- other stuff ...

    send_status(session, Stype.Logic, Cmd.eLogicLoginRes, uid, Responses.OK)
end


local game_manager = {
    logic_login = logic_login
}

return game_manager
