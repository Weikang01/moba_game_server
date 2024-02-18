Logger.init("log/test/", "test", true, -5)

local mysql_auth_center = require("test/test")

Scheduler.once(function()
    print("Start guest_account_upgrade!")
    mysql_auth_center.get_uinfo_from_uid(32, function(err, uinfo)
        if err then
            Logger.error("error in get_uinfo_from_uid!")
        end

        print("uinfo.is_guest: " .. tostring(uinfo.is_guest))

        if uinfo.is_guest == 1 then
            mysql_auth_center.guest_account_upgrade(32, "1", "C4CA4238A0B923820DCC509A6F75849B", function(err, ret)
                if err then
                    Logger.error("error in guest_account_upgrade!")
                end
            end)
        end
    end)
end, 1000)
