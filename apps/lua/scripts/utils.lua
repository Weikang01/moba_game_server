local function _print_table(lua_table, indent, result)
    indent = indent or 0
    result = result or ""
    for k, v in pairs(lua_table) do
        if type(k) == "string" then
            k = string.format("%q", k)
        end
        local szSuffix = ""
        if type(v) == "table" then
            szSuffix = "{"
        end
        local szPrefix = string.rep("    ", indent)
        local formatting = szPrefix .. k .. " = " .. szSuffix
        if type(v) == "table" then
            result = result .. formatting .. "\n"
            result = print_table(v, indent + 1, result)
            result = result .. szPrefix .. "},\n"
        else
            local szValue = ""
            if type(v) == "string" then
                szValue = string.format("%q", v)
            else
                szValue = tostring(v)
            end
            result = result .. formatting .. szValue .. ",\n"
        end
    end
    return result
end

local function print_table(lua_table)
    return "{\n" .. _print_table(lua_table, 1) .. "}\n"
end

return {
    print_table = print_table
}
