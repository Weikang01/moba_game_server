local Cmd = require("cmd")
local Guest = require("auth.guest")
local EditProfile = require("auth.edit_profile")
local AccountUpgrade = require("auth.account_upgrade")
local UnameLogin = require("auth.uname_login")
local Logout = require("auth.logout")

local auth_service_handlers = {}
auth_service_handlers[Cmd.eGuestLoginReq] = Guest.login
auth_service_handlers[Cmd.eEditProfileReq] = EditProfile.do_edit_profile
auth_service_handlers[Cmd.eGuestUpgradeReq] = AccountUpgrade.account_upgrade
auth_service_handlers[Cmd.eUserLoginReq] = UnameLogin.login
auth_service_handlers[Cmd.eLogoutReq] = Logout.logout


local function session_recv_cmd(s, cmd_msg)
    if auth_service_handlers[cmd_msg[2]] then
        auth_service_handlers[cmd_msg[2]](s, cmd_msg)
    end
end

local function session_disconnect(s, stype)
    print("auth server disconnected!")
end


local auth_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

return auth_service
