local Stype = require("stype")
local Cmd = require("cmd")
local mysql_moba_game = require("db.mysql_moba_game")
local Responses = require("responses")

-- {stype, ctype, utag, body}
local function do_get_ugame_info(session, cmd_msg)
    local uid = cmd_msg[3]
    mysql_moba_game.get_ugame_info(uid, function(err, uinfo)
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

        if uinfo == nil then
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

        if uinfo.ustatus ~= 0 then -- user is blocked
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

        -- print("guest login success! uid: " .. uinfo.uid .. "\tunick: " .. uinfo.unick) -- login success, return to client
        Session.send_msg(session, {
            stype = Stype.System,
            ctype = Cmd.eGetUGameInfoRes,
            utag = cmd_msg[3],
            body = {
                status = Responses.OK,
                uinfo = {
                    uchip  = uinfo.uchip,
                    uexp   = uinfo.uexp,
                    uvip   = uinfo.uvip,
                    uchip2 = uinfo.uchip2,
                    uchip3 = uinfo.uchip3,
                    udata1 = uinfo.udata1,
                    udata2 = uinfo.udata2,
                    udata3 = uinfo.udata3,
                }
            }
        })
    end)
end

local _get_ugame_info = {
    get_ugame_info = do_get_ugame_info
}

return _get_ugame_info
