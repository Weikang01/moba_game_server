local session_set = {}  -- save all sessions

function broadcast_except(msg, except)
	for i = 1, #session_set do
		if (session_set[i] ~= except) then
			session.send_msg(session_set[i], msg)
		end
	end
end

local function on_recv_login_cmd(s)
	-- save session if not exist
	for i = 1, #session_set do
		if (session_set[i] == s) then
			-- send to client
			session.send_msg(s, {
				stype = 1,
				ctype = 2,
				utag  = 0,
				body  = {
					status = -1  -- already exists
				}
			})
			return
		end
	end
	
	-- add session
	table.insert(session_set, s)
	-- send to client
	session.send_msg(s, {
		stype = 1,
		ctype = 2,
		utag  = 0,
		body  = {
			status = 1  -- success
		}
	})

	local s_ip, s_port = session.get_address(s)

	-- broadcast to all
	broadcast_except({
		stype = 1,
		ctype = 7,
		utag  = 0,
		body  = {
			ip   = s_ip,
			port = s_port
		}
	}, s)
end

local function on_recv_exit_cmd(s)
	-- remove session if exist
	for i = 1, #session_set do
		if (session_set[i] == s) then
			-- send to client
			session.send_msg(s, {
				stype = 1,
				ctype = 4,
				utag  = 0,
				body  = {
					status = 1  -- success
				}
			})
			
			local s_ip, s_port = session.get_address(s)

			-- broadcast to all
			broadcast_except({
				stype = 1,
				ctype = 8,
				utag  = 0,
				body  = {
					ip   = s_ip,
					port = s_port
				}
			}, s)

			-- remove session
			table.remove(session_set, i)
			return
		end
	end

	-- send to client
	session.send_msg(s, {
		stype = 1,
		ctype = 4,
		utag  = 0,
		body  = {
			status = -1  -- not exists
		}
	})
end

local function on_recv_send_msg_cmd(s, str)
	for i = 1, #session_set do
		if (session_set[i] == s) then
			-- send to client
			session.send_msg(s, {
				stype = 1,
				ctype = 6,
				utag  = 0,
				body  = {
					status = 1  -- success
				}
			})

			local s_ip, s_port = session.get_address(s)

			-- broadcast to all
			broadcast_except({
				stype = 1,
				ctype = 9,
				utag  = 0,
				body  = {
					ip   = s_ip,
					port = s_port,
					content  = str
				}
			}, s)
			return
		end
	end

	-- send to client
	session.send_msg(s, {
		stype = 1,
		ctype = 6,
		utag  = 0,
		body  = {
			status = -1  -- not exists
		}
	})
end


local function session_recv_cmd(s, cmd_msg)
    -- print(cmd_msg[1]) -- stype
    -- print(cmd_msg[2]) -- ctype
    -- print(cmd_msg[3]) -- utag
	-- print(cmd_msg[4]) -- body

    local ctype = cmd_msg[2]
    if (ctype == 1) then
		-- login
        on_recv_login_cmd(s)
	elseif (ctype == 3) then
        -- exit
		on_recv_exit_cmd(s)
    elseif (ctype == 5) then
        -- send msg
		local body = cmd_msg[4]
		on_recv_send_msg_cmd(s, body.content)
    end
end

local function session_disconnect(s)
    local s_ip, s_port = session.get_address(s)
    
	-- remove session if exist
	for i = 1, #session_set do
		if (session_set[i] == s) then
			-- broadcast to all
			broadcast_except({
				stype = 1,
				ctype = 8,
				utag  = 0,
				body  = {
					ip   = s_ip,
					port = s_port
				}
			}, s)

			-- remove session
			table.remove(session_set, i)
			return
		end
	end
end

local chatroom_service = {
    on_session_recv_cmd = session_recv_cmd,
    on_session_disconnect = session_disconnect
}

local chatroom_server = {
    stype = 1,
    service = chatroom_service
}

return chatroom_server
