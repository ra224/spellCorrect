#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
//#include <stdlib.h>

using namespace std;

class InetAddress 
{
public:
	explicit InetAddress(short port);
	InetAddress(const char * pIp, short port);
	InetAddress(const struct sockaddr_in & addr);

	void setSockAddrInet(const struct sockaddr_in &addr)
	{
		addr_ = addr;
	}
	const struct sockaddr_in * getSockAddrPtr() const;

	string ip() const;
	unsigned short port() const;

private:
	struct sockaddr_in addr_;	
};

#endif
