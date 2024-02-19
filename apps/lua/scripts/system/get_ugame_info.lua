local Stype = require("stype")
local Cmd = require("cmd")
local mysql_moba_game = require("db.mysql_moba_game")
local Responses = require("responses")
local redis_game = require("db.redis_game")

-- {stype, ctype, utag, body}
local function do_get_ugame_info(session, cmd_msg)
    local uid = cmd_msg[3]
    mysql_moba_game.get_ugame_info(uid, function(err, ugameinfo)
        if err then
            Session.send_msg(session, {
                stype = Stype.System,
                ctype = Cmd.eGetUGameInfoRes,
                utag = uid,
                body = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end

        if ugameinfo == nil then
            mysql_moba_game.insert_ugame_info(uid, function(erro, ret)
                if erro then
                    Session.send_msg(session, {
                        stype = Stype.System,
                        ctype = Cmd.eGetUGameInfoRes,
                        utag = uid,
                        body = {
                            status = Responses.SYSTEM_ERR
                        }
                    })
                    return
                end
                do_get_ugame_info(session, cmd_msg)
            end)
            return
        end

        if ugameinfo.ustatus ~= 0 then -- user is blocked
            Session.send_msg(session, {
                stype = Stype.System,
                ctype = Cmd.eGetUGameInfoRes,
                utag = uid,
                body = {
                    status = Responses.USER_BLOCKED
                }
            })
            return
        end
        -- redis_center.set_uinfo_to_redis(uinfo.uid, uinfo)
        redis_game.set_ugameinfo(uid, ugameinfo)

        -- print("guest login success! uid: " .. uinfo.uid .. "\tunick: " .. uinfo.unick) -- login success, return to client
        Session.send_msg(session, {
            stype = Stype.System,
            ctype = Cmd.eGetUGameInfoRes,
            utag = cmd_msg[3],
            body = {
                status = Responses.OK,
                uinfo = {
                    uchip  = ugameinfo.uchip,
                    uexp   = ugameinfo.uexp,
                    uvip   = ugameinfo.uvip,
                    uchip2 = ugameinfo.uchip2,
                    uchip3 = ugameinfo.uchip3,
                    udata1 = ugameinfo.udata1,
                    udata2 = ugameinfo.udata2,
                    udata3 = ugameinfo.udata3,
                }
            }
        })
    end)
end

local _get_ugame_info = {
    get_ugame_info = do_get_ugame_info
}

return _get_ugame_info
