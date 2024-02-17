local mysql_auth_center = require("db/mysql_auth_center")

local function login(session, cmd_msg)
    local g_key = cmd_msg[4].guest_key
    mysql_auth_center.get_guest_uinfo(g_key, function(err, uinfo)
        if err then
            print("an error occurred in get_guest_uinfo!")
            return
        end

        if uinfo == nil then
            mysql_auth_center.insert_guest_user(g_key, function(erro, ret)
                if erro then
                    print("an error occurred in insert_guest_user!")
                    return
                end
                login(session, cmd_msg)
            end)
            return
        end

        if uinfo.status ~= 0 then -- user is not active
            return
        end
        if uinfo.is_guest ~= 1 then -- account not guest anymore
            return
        end

        -- print("guest login success! uid: " .. uinfo.uid .. "\tunick: " .. uinfo.unick) -- login success, return to client
    end)
end


local guest = {
    login = login,
}

return guest
