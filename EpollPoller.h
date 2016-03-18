#ifndef _EPOLLPOLLER_H
#define _EPOLLPOLLER_H

#include "Noncopyable.h"
#include "TcpConnection.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <assert.h>

using namespace std;

class EpollPoller : Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	explicit EpollPoller (int listenfd);
	~EpollPoller();

	void loop();
	void unloop();

	void setConnectionCallback (EpollCallback cb);
	void setMessageCallback (EpollCallback cb);
	void setCloseCallback (EpollCallback cb);

private:
	void waitEpollfd();
	void handleConnection();
	void handleMessage(int peerfd);

private:
	int epollfd_;
	int listenfd_;
	bool isLooping_;

	typedef vector<struct epoll_event> EventList;
	EventList eventsList_;
	
	typedef map<int, TcpConnectionPtr> ConnectionMap;
	ConnectionMap connMap_;

	EpollCallback onConnectionCb_;
	EpollCallback onMessageCb_;
	EpollCallback onCloseCb_;
	
};


#endif
