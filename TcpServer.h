#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "Noncopyable.h"
#include "EpollPoller.h"
#include "Socket.h"

class TcpServer : Noncopyable
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;

	explicit TcpServer(const InetAddress &addr);

	void start()
	{
		poller_.loop();
	}

	void setConnectionCallback(TcpServerCallback cb)
	{
		poller_.setConnectionCallback(move(cb));	
	}

	void setMessageCallback(TcpServerCallback cb)
	{
		poller_.setMessageCallback(move(cb));	
	}

	void setCloseCallback(TcpServerCallback cb)
	{
		poller_.setCloseCallback(move(cb));	
	}

private:
	Socket sockfd_;
	EpollPoller poller_;
};


#endif
