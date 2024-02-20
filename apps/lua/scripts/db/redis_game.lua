local game_config = require("game_config")

local redis_conn = nil

local function is_connected()
    if not redis_conn then
        return false
    end
    return true
end


local function redis_connect_to_game()
    local redis_config = game_config.game_redis
    Redis.connect(redis_config.host, redis_config.port, function(err, context)
        if err then
            Logger.error("connect to redis game db failed! err: " .. err)
            Scheduler.once(redis_connect_to_game, 5000)
            return
        else
            redis_conn = context

            local sql = "select " .. redis_config.db_index;
            Redis.query(redis_conn, sql, function(erro, ret)
                if erro then
                    Logger.error("connect to redis game db failed! err: " .. err)
                    Scheduler.once(redis_connect_to_game, 5000)
                end

                Logger.debug("connect to redis db successed!")
            end)
        end
    end)
end

local function set_ugameinfo_to_redis(uid, ugameinfo)
    if redis_conn == nil then
        Logger.error("redis game disconnected")
        return
    end

    local redis_cmd = "hmset moba_auth_game_user_uid_" .. uid ..
        " uchip " .. ugameinfo.uchip ..
        " uexp " .. ugameinfo.uexp ..
        " uvip " .. ugameinfo.uvip

    Redis.query(redis_conn, redis_cmd, function(err, ret)
    end)
end

-- ret_handler(err, uinfo)
local function get_ugameinfo_to_redis(uid, ret_handler)
    if redis_conn == nil then
        if ret_handler then
            ret_handler("redis connection failed!", nil)
        end
        return
    end

    local redis_cmd = "hgetall moba_auth_game_user_uid_" .. uid

    Redis.query(redis_conn, redis_cmd, function(err, ret)
        if err then
            if ret_handler then
                ret_handler(err, nil)
            end
            return
        end

        local ugameinfo = {
            uid   = uid,
            uchip = tonumber(ret[4]),
            uexp  = tonumber(ret[6]),
            uvip  = tonumber(ret[8]),
        }

        if ret_handler then
            ret_handler(nil, ugameinfo)
        end
    end)
end


local function add_chip_to_redis(uid, add_chip)
    get_ugameinfo_to_redis(uid, function(err, uinfo)
        if err then
            Logger.error("Get uinfo in redis failed!")
            return
        end

        uinfo.uchip = uinfo.uchip + add_chip

        set_ugameinfo_to_redis(uid, uinfo)
    end)
end


redis_connect_to_game()

local redis_game = {
    is_connected  = is_connected,
    set_ugameinfo = set_ugameinfo_to_redis,
    get_ugameinfo = get_ugameinfo_to_redis,
    add_chip      = add_chip_to_redis
}

return redis_game
