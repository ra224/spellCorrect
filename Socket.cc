#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>

using namespace std;

Socket::Socket(int sockfd)
	: sockfd_(sockfd)
{
}

/*
Socket::Socket()
{
	sockfd_ = socket(AF_INET , SOCK_STREAM, 0);
	if (-1 == sockfd_)
		perror("socket error");
}
*/

Socket::~Socket()
{
	close(sockfd_);
}

/*
void Socket::ready(const InetAddress & addr)
{
	setReuseAddr(true);
	setReusePort(true);
	bindAddress(addr);
	listen();
}
*/


void Socket::bindAddress(const InetAddress & addr)
{
	if ( -1 == bind(sockfd_, (struct sockaddr *)(addr.getSockAddrPtr()) , sizeof(InetAddress)  ) )
	{
		fprintf(stderr, "bind address error\n");
		exit(EXIT_FAILURE);
	}
}

void Socket::listen()
{
	if (-1 == ::listen(sockfd_, 10) )
	{
		fprintf(stderr, "listen error\n");
		exit(EXIT_FAILURE);
	}
}

int Socket::accept()
{
	int peerfd = ::accept(sockfd_, NULL, NULL);
	if (-1 == peerfd)
	{
		fprintf(stderr,"accept error\n");
		exit(EXIT_FAILURE);
	}
	return peerfd;
}

void Socket::shutdownWrite()
{
	if ( -1 == shutdown(sockfd_, SHUT_WR) )
	{
		fprintf(stderr,"shutdown write error\n");
		exit(EXIT_FAILURE);
	}
}

void Socket::setTcpNoDelay(bool flag)
{
	int opt = (flag ? 1 : 0);
	if (  setsockopt(sockfd_ , 
					IPPROTO_TCP, 
					TCP_NODELAY, 
					(const char*)&opt, 
					sizeof(opt)) == -1  )
	{
		fprintf(stderr, "setTcpNoDelay error\n");
		exit(EXIT_FAILURE);
	}
}

void Socket::setReuseAddr(bool flag)
{
	int opt = (flag ? 1 : 0);
	if (  setsockopt(sockfd_ , 
					SOL_SOCKET, 
					SO_REUSEADDR, 
					(const char*)&opt, 
					sizeof(opt)) == -1  )
	{
		fprintf(stderr, "setReuseAddr error\n");
		exit(EXIT_FAILURE);
	}
}

void Socket::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	int opt = (flag ? 1 : 0);
	if (  setsockopt(sockfd_ , 
					SOL_SOCKET, 
					SO_REUSEPORT, 
					(const char*)&opt, 
					sizeof(opt)) == -1  )
	{
		perror("setsockopt reuseport error");
		close(sockfd_);
		exit(EXIT_FAILURE);
	}
#else
	if (flag)
	{
		fprintf(stderr,"SO_REUSEPORT is not supported!\n");
	}
#endif
}

void Socket::setKeepAlive(bool flag)
{
	int opt = (flag ? 1 : 0);
	if (  setsockopt(sockfd_ , 
					SOL_SOCKET, 
					SO_KEEPALIVE, 
					(const char*)&opt, 
					sizeof(opt)) == -1  )
	{
		fprintf(stderr, "setKeepAlive error\n");
		exit(EXIT_FAILURE);
	}
}

InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if (getsockname(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		fprintf(stderr, "getsockname error\n");
		exit(EXIT_FAILURE);
	}
	return InetAddress(addr);
}
	
InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if (getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
	{
		fprintf(stderr, "getpeername error\n");
		exit(EXIT_FAILURE);
	}
	return InetAddress(addr);
}	
