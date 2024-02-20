Logger.init("log/test/", "test", true, -5)

local game_config = require("game_config")
local test_db_index = 4
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
            Logger.error("connect to redis test db failed! err: " .. err)
            Scheduler.once(redis_connect_to_game, 5000)
            return
        else
            redis_conn = context
            local sql = "select " .. test_db_index;
            Redis.query(redis_conn, sql, function(erro, ret)
                if erro then
                    Logger.error("connect to redis test db failed! err: " .. err)
                    Scheduler.once(redis_connect_to_game, 5000)
                end

                Logger.debug("connect to test redis db successed!")
            end)
        end
    end)
end

local function set_ugameinfo_to_redis(uid, ugameinfo)
    if redis_conn == nil then
        Logger.error("redis game disconnected")
        return
    end

    local redis_cmd = "hmset test_data_" .. uid ..
        " uchip " .. ugameinfo.uchip ..
        " uexp " .. ugameinfo.uexp ..
        " uvip " .. ugameinfo.uvip

    print(redis_cmd)

    Redis.query(redis_conn, redis_cmd, function(err, ret)
        if err then
            Logger.error("redis query error: " .. err)
        end

        print("test_data_" .. uid .. " set in redis!")
    end)
end

redis_connect_to_game()

local function test()
    for i = 1, 2, 1 do
        set_ugameinfo_to_redis(10006 + i, {
            uchip = 100,
            uexp  = 200,
            uvip  = 300
        })
    end
end

Scheduler.once(test, 1000)
