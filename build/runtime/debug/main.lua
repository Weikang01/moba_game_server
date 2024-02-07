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

redis_wrapper.connect("127.0.0.1", 6379, function(err, context)
    if (err) then
        print(err)
        return
    end

    redis_wrapper.query(context, "hgetall 002001", function(err, result)
        if (err) then
            print(error)
            return
        end

        print("success")
        PrintTable(result)
    end)
end)
