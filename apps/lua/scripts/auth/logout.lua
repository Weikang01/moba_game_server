local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_auth_center = require("db/mysql_auth_center")
local redis_center = require("db/redis_center")

-- {stype, ctype, utag, body}
local function do_logout(session, cmd_msg)
    local uid = cmd_msg[3]

    -- statistic logics

    Session.send_msg(session, {
        stype = Stype.Auth,
        ctype = Cmd.eLogoutRes,
        utag = uid,
        body = {
            status = Responses.OK
        }
    })
end

local logout = {
    logout = do_logout,
}

return logout
