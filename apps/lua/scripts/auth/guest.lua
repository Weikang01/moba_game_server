local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_auth_center = require("db/mysql_auth_center")
local redis_center = require("db/redis_center")

local function is_valid_gkey(g_key)
    if type(g_key) ~= "string" or string.len(g_key) ~= 32 then
        return false
    end
    return true
end

local function login(session, cmd_msg)
    local g_key = cmd_msg[4].guest_key
    if not (is_valid_gkey(g_key)) then
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eGuestLoginRes,
            utag = cmd_msg[3],
            body = {
                status = Responses.INVALID_PARAMS
            }
        })
        return
    end

    mysql_auth_center.get_guest_uinfo(g_key, function(err, uinfo)
        if err then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end

        if uinfo == nil then
            mysql_auth_center.insert_guest_user(g_key, function(erro, ret)
                if erro then
                    Session.send_msg(session, {
                        stype = Stype.Auth,
                        ctype = Cmd.eGuestLoginRes,
                        utag = cmd_msg[3],
                        body = {
                            status = Responses.SYSTEM_ERR
                        }
                    })
                    return
                end
                login(session, cmd_msg)
            end)
            return
        end

        if uinfo.status ~= 0 then -- user is blocked
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.USER_BLOCKED
                }
            })
            return
        end
        if uinfo.is_guest ~= 1 then -- account not guest anymore
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestLoginRes,
                utag = cmd_msg[3],
                body = {
                    status = Responses.USER_IS_NOT_GUEST
                }
            })
            return
        end

        redis_center.set_uinfo_to_redis(uinfo.uid, uinfo)

        -- print("guest login success! uid: " .. uinfo.uid .. "\tunick: " .. uinfo.unick) -- login success, return to client
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eGuestLoginRes,
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


local guest = {
    login = login,
}

return guest
