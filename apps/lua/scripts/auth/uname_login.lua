local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_auth_center = require("db/mysql_auth_center")
local redis_center = require("db/redis_center")

-- {stype, ctype, utag, body}
local function login(session, cmd_msg)
    local utag = cmd_msg[3]
    local body = cmd_msg[4]
    local uname = body.uname
    local upwd_md5 = body.upwd_md5

    if string.len(upwd_md5) ~= 32 or string.len(uname) <= 0 then
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eUserLoginRes,
            utag  = utag,
            body  = {
                status = Responses.INVALID_PARAMS
            }
        })
        return
    end

    mysql_auth_center.get_uinfo_by_uname_upwd(uname, upwd_md5, function(err, uinfo)
        if err then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eUserLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end

        if uinfo == nil then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eUserLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.USER_NOT_EXISTS
                }
            })
            return
        end

        if uinfo.status ~= 0 then -- user is blocked
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eUserLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.USER_BLOCKED
                }
            })
            return
        end

        redis_center.set_uinfo_to_redis(uinfo.uid, uinfo)

        -- print("guest login success! uid: " .. uinfo.uid .. "\tunick: " .. uinfo.unick) -- login success, return to client
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eUserLoginRes,
            utag = cmd_msg[3],
            body = {
                status = Responses.OK,
                uinfo = {
                    unick = uinfo.unick,
                    usex = uinfo.usex,
                    usysavatar = uinfo.usysavatar,
                    uvip = uinfo.uvip,
                    uid = uinfo.uid,
                }
            }
        })
    end)
end

local uname_login = {
    login = login,
}

return uname_login
