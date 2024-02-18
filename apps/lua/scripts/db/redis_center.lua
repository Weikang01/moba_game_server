local game_config = require("game_config")

local redis_conn = nil
local function redis_connect_to_center()
    local redis_config = game_config.center_redis
    Redis.connect(redis_config.host, redis_config.port, function(err, context)
        if err then
            Logger.error("connect to redis failed! err: " .. err)
            Scheduler.once(redis_connect_to_center, 5000)
            return
        else
            redis_conn = context

            local sql = "select " .. redis_config.db_index;
            Redis.query(redis_conn, sql, function(erro, ret)
                if erro then
                    Logger.error("connect to redis db failed! err: " .. err)
                    Scheduler.once(redis_connect_to_center, 5000)
                end

                Logger.debug("connect to redis db successed!")
            end)
        end
    end)
end

local function set_uinfo_to_redis(uid, uinfo)
    if redis_conn == nil then
        Logger.error("redis center disconnected")
        return
    end

    local redis_cmd = "hmset moba_auth_center_user_uid_" .. uid ..
        " unick " .. uinfo.unick ..
        " usex " .. uinfo.usex ..
        " usysavatar " .. uinfo.usysavatar ..
        " uvip " .. uinfo.uvip ..
        " is_guest " .. uinfo.is_guest

    Redis.query(redis_conn, redis_cmd, function(err, ret)
    end)
end

-- ret_handler(err, uinfo)
local function get_uinfo_to_redis(uid, ret_handler)
    if redis_conn == nil then
        if ret_handler then
            ret_handler("redis connection failed!", nil)
        end
        return
    end

    local redis_cmd = "hgetall moba_auth_center_user_uid_" .. uid

    Redis.query(redis_conn, redis_cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end
        --  1) "unick"
        --  2) "g_885026"
        --  3) "usex"
        --  4) "1"
        --  5) "usysavatar"
        --  6) "2"
        --  7) "uvip"
        --  8) "0"
        --  9) "is_guest"
        -- 10) "1"
        local uinfo = {
            uid        = uid,
            unick      = ret[2],
            usex       = tonumber(ret[4]),
            usysavatar = tonumber(ret[6]),
            uvip       = tonumber(ret[8]),
            is_guest   = tonumber(ret[10])
        }

        if ret_handler then
            ret_handler(nil, uinfo)
        end
    end)
end


local function edit_profile(uid, unick, usex, usysavatar)
    get_uinfo_to_redis(uid, function(err, uinfo)
        if err then
            Logger.error("Get uinfo in redis failed!")
            return
        end

        uinfo.unick = unick
        uinfo.usex = usex
        uinfo.usysavatar = usysavatar

        set_uinfo_to_redis(uid, uinfo)
    end)
end


redis_connect_to_center()

local redis_center = {
    set_uinfo_to_redis = set_uinfo_to_redis,
    get_uinfo_to_redis = get_uinfo_to_redis,
    edit_profile       = edit_profile
}

return redis_center
