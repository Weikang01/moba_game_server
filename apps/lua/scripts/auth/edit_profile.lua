local Stype = require("stype")
local Cmd = require("cmd")
local Responses = require("responses")
local mysql_auth_center = require("db/mysql_auth_center")

-- {stype, ctype, utag, body}
local function do_edit_profile(session, cmd_msg)
    local uid = cmd_msg[3]
    local edit_profile_req = cmd_msg[4]

    print("unick: " ..
        edit_profile_req.unick .. "usex: " .. edit_profile_req.usex .. "usysavatar: " .. edit_profile_req.usysavatar)

    if string.len(edit_profile_req.unick) <= 0 or
        edit_profile_req.usex < 0 or edit_profile_req.usex > 2 or
        edit_profile_req.usysavatar < 0 or edit_profile_req.usysavatar > 9
    then -- parameter error
        Session.send_msg(session, {
            stype = Stype.Auth,
            ctype = Cmd.eEditProfileRes,
            utag  = uid,
            body  = {
                status = Responses.INVALID_PARAMS
            }
        })
    end

    mysql_auth_center.edit_profile(uid, edit_profile_req.unick, edit_profile_req.usex, edit_profile_req.usysavatar,
        function(err, ret)
            if err then
                Session.send_msg(session, {
                    stype = Stype.Auth,
                    ctype = Cmd.eEditProfileRes,
                    utag  = uid,
                    body  = {
                        status = Responses.SYSTEM_ERR
                    }
                })
            else
                Session.send_msg(session, {
                    stype = Stype.Auth,
                    ctype = Cmd.eEditProfileRes,
                    utag  = uid,
                    body  = {
                        status = Responses.OK
                    }
                })
            end
        end)
end

local edit_profile = {
    do_edit_profile = do_edit_profile,
}

return edit_profile
