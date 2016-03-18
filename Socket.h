#ifndef _SOCKET_H
#define _SOCKET_H

#include "Noncopyable.h"
#include "InetAddress.h"

class Socket : Noncopyable
{
public:
	explicit Socket(int sockfd);
	~Socket();

	int fd() const {return sockfd_;}
	void bindAddress(const InetAddress &addr);
	void listen();
	int accept();

	void shutdownWrite();

	void setTcpNoDelay(bool flag);
	void setReuseAddr(bool flag);
	void setReusePort(bool flag);
	void setKeepAlive(bool flag);
	
	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);

private:
	const int sockfd_;
};



#endif
