local Stype = require("stype")

local remote_servers = {}
remote_servers[Stype.Auth] = {
    stype = Stype.Auth,
    ip = "127.0.0.1",
    port = 8000,
    desc = "Auth server"
}
--[[
remote_servers[Stype.System] = {
    stype = Stype.System,
    ip = "127.0.0.1",
    port = 8001,
    desc = "System server"
}
]]

local game_config = {
    gateway_tcp_ip = "127.0.0.1",
    gateway_tcp_port = 6080,
    gateway_ws_ip = "127.0.0.1",
    gateway_ws_port = 6081,

    servers = remote_servers,

    auth_mysql = {
        host = "127.0.0.1",
        port = 3306,
        db_name = "auth_center",
        uname = "root",
        upwd = "123"
    }
}

return game_config
