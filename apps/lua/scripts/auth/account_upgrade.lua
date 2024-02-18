local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_auth_center = require("db/mysql_auth_center")
local redis_center = require("db/redis_center")

local function update_account(session, uid, uname, upwd_md5)
    -- print("update account!")
    mysql_auth_center.guest_account_upgrade(uid, uname, upwd_md5, function(err, ret)
        if err then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eGuestUpgradeRes,
            utag  = uid,
            body  = {
                status = Responses.OK
            }
        })
    end)
end

local function check_guest_account_valid(session, uid, uname, upwd_md5)
    mysql_auth_center.get_uinfo_from_uid(uid, function(err, uinfo)
        if err then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end

        if uinfo == nil then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.USER_NOT_EXISTS
                }
            })
            return
        end

        if uinfo.is_guest ~= 1 then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.USER_IS_NOT_GUEST
                }
            })
            return
        else
            update_account(session, uid, uname, upwd_md5)
        end
    end)
end

-- {stype, ctype, uid, body}
local function do_account_upgrade(session, cmd_msg)
    local uid = cmd_msg[3]
    local uname = cmd_msg[4].uname
    local upwd_md5 = cmd_msg[4].upwd_md5

    if string.len(uname) <= 0 or string.len(upwd_md5) ~= 32 then
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eGuestUpgradeRes,
            utag  = uid,
            body  = {
                status = Responses.INVALID_PARAMS
            }
        })
    end

    mysql_auth_center.check_uname_exists(uname, function(err, ret)
        if err then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.SYSTEM_ERR
                }
            })
            return
        end

        local Utils = require("utils")

        if ret then
            Session.send_msg(session, {
                stype = Stype.Auth,
                ctype = Cmd.eGuestUpgradeRes,
                utag  = uid,
                body  = {
                    status = Responses.USERNAME_EXISTS
                }
            })
        else
            check_guest_account_valid(session, uid, uname, upwd_md5)
        end
    end)
end


local account_upgrade = {
    account_upgrade = do_account_upgrade,
}

return account_upgrade
