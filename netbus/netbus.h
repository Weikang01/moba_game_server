#ifndef __NETBUS_H__
#define __NETBUS_H__

class Session;

class Netbus
{
public:
	static Netbus* instance();

public:
	void init();
	void tcp_listen(int port);
	void ws_listen(int port);
	void udp_listen(int port);
	void run();
	void tcp_connect(const char* host, int port, void(*connect_cb)(int err, Session* session, void* udata), void* udata);
};
#endif // !__NETBUS_H__
