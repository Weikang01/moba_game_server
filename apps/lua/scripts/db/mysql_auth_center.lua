local game_config = require("game_config")

local mysql_conn = nil
local function mysql_connect_to_auth_center()
    local auth_config = game_config.auth_mysql
    MySQL.connect(auth_config.host, auth_config.port, auth_config.uname, auth_config.upwd, auth_config.db_name,
        function(err, context)
            if err then
                Logger.error("connect to auth center db failed! err: " .. err)
                Scheduler.once(mysql_connect_to_auth_center, 5000)
                return
            else
                Logger.debug("connect to auth center db successed!")
                mysql_conn = context
            end
        end)
end

local function get_uinfo_from_uid(uid, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end
    local sql =
    "select uid, unick, usex, usysavatar, uvip, status, is_guest from user_info where uid = %d limit 1"
    local cmd = string.format(sql, uid)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end

        -- record not found
        if ret == nil or #ret <= 0 then
            if ret_handler then
                ret_handler(nil, nil)
            end
            return
        end

        if ret_handler then
            local uinfo = {
                uid        = tonumber(ret[1].uid),
                unick      = tostring(ret[1].unick),
                usex       = tonumber(ret[1].usex),
                usysavatar = tonumber(ret[1].usysavatar),
                uvip       = tonumber(ret[1].uvip),
                status     = tonumber(ret[1].status),
                is_guest   = tonumber(ret[1].is_guest)
            }

            ret_handler(nil, uinfo)
        end
    end)
end

local function get_guest_uinfo(guest_key, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end
    local sql =
    "select uid, unick, usex, usysavatar, uvip, status, is_guest from user_info where guest_key = \"%s\" limit 1"
    local cmd = string.format(sql, guest_key)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end

        -- record not found
        if ret == nil or #ret <= 0 then
            if ret_handler then
                ret_handler(nil, nil)
            end
            return
        end

        if ret_handler then
            local uinfo = {
                uid = tonumber(ret[1].uid),
                unick = tostring(ret[1].unick),
                usex = tonumber(ret[1].usex),
                usysavatar = tonumber(ret[1].usysavatar),
                uvip = tonumber(ret[1].uvip),
                status = tonumber(ret[1].status),
                is_guest = tonumber(ret[1].is_guest)
            }

            ret_handler(nil, uinfo)
        end
    end)
end

local function insert_guest_user(guest_key, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local unick = 'g_' .. math.random(100000, 999999)
    local usex = math.random(0, 2)
    local usysavatar = math.random(0, 9)

    local sql =
    "insert into user_info (`unick`, `usex`, `usysavatar`, `is_guest`, `guest_key`) values (\"%s\", %d, %d, 1, \"%s\")"
    local cmd = string.format(sql, unick, usex, usysavatar, guest_key)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
        else
            if ret_handler then
                ret_handler(nil, nil)
            end
        end
    end)
end

local function edit_profile(uid, unick, usex, usysavatar, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql =
    "update user_info set `unick`=\"%s\", `usex`=%d, `usysavatar`=%d where (`uid`=%d)"
    local cmd = string.format(sql, unick, usex, usysavatar, uid)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
        else
            if ret_handler then
                ret_handler(nil, nil)
            end
        end
    end)
end

local function get_uinfo_by_uname_upwd(uname, upwd_md5, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql =
    "select uid, unick, usex, usysavatar, uvip, status, is_guest from user_info where uname = \"%s\" and upwd = \"%s\" limit 1"
    local cmd = string.format(sql, uname, upwd_md5)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end

        -- record not found
        if ret == nil or #ret <= 0 then
            if ret_handler then
                ret_handler(nil, nil)
            end
            return
        end

        if ret_handler then
            local uinfo = {
                uid        = tonumber(ret[1].uid),
                unick      = tostring(ret[1].unick),
                usex       = tonumber(ret[1].usex),
                usysavatar = tonumber(ret[1].usysavatar),
                uvip       = tonumber(ret[1].uvip),
                status     = tonumber(ret[1].status),
                is_guest   = tonumber(ret[1].is_guest)
            }

            ret_handler(nil, uinfo)
        end
    end)
end

local function check_uname_exists(uname, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql = "select uid from user_info where uname =\"%s\""
    local cmd = string.format(sql, uname)

    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end

        if ret == nil or #ret <= 0 then
            if ret_handler then
                ret_handler(nil, nil)
            end
            return
        end

        if ret_handler then
            ret_handler(nil, {
                uid = tonumber(ret[1].uid)
            })
        end
    end)
end

local function guest_account_upgrade(uid, uname, upwd_md5, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql = "update user_info set `uname`=\"%s\", `upwd`=\"%s\", `is_guest`=0 where (`uid`=%d)"
    local cmd = string.format(sql, uname, upwd_md5, uid)
    MySQL.query(mysql_conn, cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
        else
            if ret_handler then
                ret_handler(nil, nil)
            end
        end
    end)
end


mysql_connect_to_auth_center()

local mysql_auth_center = {
    get_guest_uinfo         = get_guest_uinfo,
    insert_guest_user       = insert_guest_user,
    edit_profile            = edit_profile,
    check_uname_exists      = check_uname_exists,
    guest_account_upgrade   = guest_account_upgrade,
    get_uinfo_from_uid      = get_uinfo_from_uid,
    get_uinfo_by_uname_upwd = get_uinfo_by_uname_upwd,
}

return mysql_auth_center
