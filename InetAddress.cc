#include "InetAddress.h"

static_assert(sizeof(InetAddress) == sizeof(struct sockaddr_in), "InetAddress Error");

InetAddress::InetAddress(short port)
{
	memset(&addr_,0,sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = INADDR_ANY;
}

InetAddress::InetAddress(const char *pIp , short port)
{
	memset(&addr_,0,sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = inet_addr(pIp);
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
	:	addr_(addr)
{
}

const struct sockaddr_in * InetAddress::getSockAddrPtr() const
{
	return &addr_;
}

unsigned short InetAddress::port() const
{
	return ntohs(addr_.sin_port);
}

string InetAddress::ip() const
{
	return  string(inet_ntoa(addr_.sin_addr));
}
