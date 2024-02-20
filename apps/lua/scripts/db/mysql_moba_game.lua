local game_config = require("game_config")
local moba_game_config = require("moba_game_config")
local mysql_conn = nil

local function is_connected()
    if not mysql_conn then
        return false
    end
    return true
end

local function mysql_connect_to_moba_db()
    local moba_config = game_config.moba_mysql
    MySQL.connect(moba_config.host, moba_config.port, moba_config.uname, moba_config.upwd, moba_config.db_name,
        function(err, context)
            if err then
                Logger.error("connect to moba game db failed! err: " .. err)
                Scheduler.once(mysql_connect_to_moba_db, 5000)
                return
            else
                Logger.debug("connect to moba game db successed!")
                mysql_conn = context
            end
        end)
end

mysql_connect_to_moba_db()


local function get_ugame_info(uid, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql =
    "select uid, uchip, uchip2, uchip3, uvip, uvip_endtime, udata1, udata2, udata3, uexp, ustatus from ugame where uid = %d limit 1"
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
                uid          = tonumber(ret[1].uid),
                uchip        = tonumber(ret[1].uchip),
                uchip2       = tonumber(ret[1].uchip2),
                uchip3       = tonumber(ret[1].uchip3),
                uvip         = tonumber(ret[1].uvip),
                uvip_endtime = tonumber(ret[1].uvip_endtime),
                udata1       = tonumber(ret[1].udata1),
                udata2       = tonumber(ret[1].udata2),
                udata3       = tonumber(ret[1].udata3),
                uexp         = tonumber(ret[1].uexp),
                ustatus      = tonumber(ret[1].ustatus),
            }
            ret_handler(nil, uinfo)
        end
    end)
end

local function insert_ugame_info(uid, ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql =
    "INSERT INTO ugame (`uid`, `uchip`, `uvip`, `uexp`) VALUES (%d, %d, %d, %d);"
    local cmd = string.format(sql, uid, moba_game_config.uinfo.uchip,
        moba_game_config.uinfo.uvip,
        moba_game_config.uinfo.uexp)
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

local function load_robot_ugame_info(ret_handler)
    if mysql_conn == nil then
        if ret_handler then
            ret_handler("mysql is not connected!", nil)
        end
        return
    end

    local sql =
    "select uid, uchip, uchip2, uchip3, uvip, uvip_endtime, udata1, udata2, udata3, uexp, ustatus from ugame where is_robot=1"
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

        if not ret_handler then
            return
        end

        local uinfos = {}
        for _, uinfo in pairs(ret) do
            local c_uinfo = {
                uid          = tonumber(uinfo.uid),
                uchip        = tonumber(uinfo.uchip),
                uchip2       = tonumber(uinfo.uchip2),
                uchip3       = tonumber(uinfo.uchip3),
                uvip         = tonumber(uinfo.uvip),
                uvip_endtime = tonumber(uinfo.uvip_endtime),
                udata1       = tonumber(uinfo.udata1),
                udata2       = tonumber(uinfo.udata2),
                udata3       = tonumber(uinfo.udata3),
                uexp         = tonumber(uinfo.uexp),
                ustatus      = tonumber(uinfo.ustatus),
            }
            table.insert(uinfos, c_uinfo)
        end

        ret_handler(nil, uinfos)
    end)
end

local mysql_moba_game = {
    is_connected          = is_connected,
    get_ugame_info        = get_ugame_info,
    insert_ugame_info     = insert_ugame_info,
    load_robot_ugame_info = load_robot_ugame_info,
}

return mysql_moba_game
