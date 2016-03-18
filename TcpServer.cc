#include "TcpServer.h"

int createSocketFd()
{
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (-1 == fd)
	{
		fprintf(stderr, "create socket fd error\n");
		exit(EXIT_FAILURE);
	}
	return fd;
}

TcpServer::TcpServer(const InetAddress &addr)
	:	sockfd_(createSocketFd()),
		poller_(sockfd_.fd())
{
	sockfd_.setTcpNoDelay(false);
	sockfd_.setReusePort(false);
	sockfd_.setKeepAlive(false);
	
	sockfd_.bindAddress(addr);
	sockfd_.listen();
}
