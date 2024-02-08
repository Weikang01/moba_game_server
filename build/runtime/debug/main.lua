logger.init("log/test", "netbus_log", true, -5)

local key = ""

function PrintTable(table, level)
    level = level or 1
    local indent = ""
    for i = 1, level do
        indent = indent .. "  "
    end

    if key ~= "" then
        print(indent .. key .. " " .. "=" .. " " .. "{")
    else
        print(indent .. "{")
    end

    key = ""
    for k, v in pairs(table) do
        if type(v) == "table" then
            key = k
            PrintTable(v, level + 1)
        else
            local content = string.format("%s%s = %s", indent .. "  ", tostring(k), tostring(v))
            print(content)
        end
    end
    print(indent .. "}")
end

local my_service = {
    on_session_recv_cmd = function(session, cmd_msg)
    end,
    on_session_disconnect = function(session)
    end
}

local ret = service.register(100, my_service);
print("ret:", ret)


local counter = 0
local timer = scheduler.schedule(function()
    print("scheduler called!")
end, 0, 1000, -1)

scheduler.once(function()
    scheduler.cancel(timer);
end, 5000)

-- mysql_wrapper.connect("127.0.0.1", 3306, "root", "123", "test_database", function(err, context)
--     if (err) then
--         print(err)
--         return
--     end

--     mysql_wrapper.query(context, "select * from families", function(err, result)
--         if (err) then
--             print(err)
--             return
--         end

--         print("success")
--         PrintTable(result)
--     end)
-- end);

-- redis_wrapper.connect("127.0.0.1", 6379, function(err, context)
--     if (err) then
--         print(err)
--         return
--     end

--     redis_wrapper.query(context, "hgetall 001001", function(err, result)
--         if (err) then
--             print(error)
--             return
--         end

--         print("success")
--         PrintTable(result)
--     end)
-- end)
