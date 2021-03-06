#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <string>
#include <string.h>
#include <memory>
#include <functional>

using namespace std;

class TcpConnection;
typedef shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection : Noncopyable,
	public enable_shared_from_this<TcpConnection>
{
public:
	typedef function<void(const TcpConnectionPtr&)> TcpConnectionCallback;
	TcpConnection(int sockfd);
	~TcpConnection();

	string receive();
	void send(const string & msg);
	void shutdown();

	int fd()
	{
		return sockfd_.fd();
	}

	string toString();

	void setConnectionCallback(TcpConnectionCallback cb);
	void setMessageCallback(TcpConnectionCallback cb);
	void setCloseCallback(TcpConnectionCallback cb);

	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();
private:
	Socket sockfd_;
	SocketIO sockIO_;
	const InetAddress localAddr_;
	const InetAddress peerAddr_;
	bool isShutdownWrite_;
	TcpConnectionCallback onConnectionCb_;
	TcpConnectionCallback onMessageCb_;
	TcpConnectionCallback onCloseCb_;
};

#endif
