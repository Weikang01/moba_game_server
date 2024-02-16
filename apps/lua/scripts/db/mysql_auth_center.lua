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


mysql_connect_to_auth_center()
