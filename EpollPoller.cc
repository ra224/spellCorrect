#include "EpollPoller.h"
#include <errno.h>

int createEpollFd()
{
	int efd = epoll_create1(0);
	if (-1 == efd)
	{
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}	
	return efd;
}

void addEpollFdRead(int efd, int fd)
{
	struct epoll_event ev;
	ev.events = EPOLL_CTL_ADD;
	ev.data.fd = fd;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
	if (-1 == ret)
	{
		perror("epoll_ctl add error");	
		exit(EXIT_FAILURE);
	}
}


void delEpollFdRead(int efd, int fd)
{
	struct epoll_event ev;
	ev.events = EPOLL_CTL_DEL;
	ev.data.fd = fd;
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
	if (-1 == ret)
	{
		perror("epoll_ctl add error");	
		exit(EXIT_FAILURE);
	}
}

int acceptConnFd(int listenfd)
{
	int peerfd = accept(listenfd, NULL, NULL);
	if (-1 == peerfd)
	{
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	return peerfd;
}

size_t recvPeek(int sockfd, void * buf, size_t len)
{
	int nread;
	do
	{
		nread = recv(sockfd, buf, len, MSG_PEEK);
	}while ( -1 == nread  &&  errno == EINTR);
	return nread;
}

bool isConnectionClosed(int sockfd)
{
	char buf[1024];
	int nread = recvPeek(sockfd, buf, sizeof(buf));
	if (-1 == nread)
	{	
		perror("recvPeek");
		exit(EXIT_FAILURE);
	}
	return (0 == nread);
}


//====================================================


EpollPoller::EpollPoller(int listenfd)
	:	epollfd_(createEpollFd()),
		listenfd_(listenfd),
		isLooping_(false),
		eventsList_(1024)
{
	addEpollFdRead(epollfd_, listenfd);
}

EpollPoller::~EpollPoller()
{
	close(epollfd_);
}

void EpollPoller::loop()
{
	isLooping_ = true;
	while (isLooping_)
		waitEpollfd();
	printf("Loop quit safely\n");
}

void EpollPoller::unloop()
{
	if (isLooping_)
		isLooping_ = false;
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	onConnectionCb_ = cb;
}	


void EpollPoller::setMessageCallback(EpollCallback cb)
{
	onMessageCb_ = cb;
}
	
void EpollPoller::setCloseCallback(EpollCallback cb)
{
	onCloseCb_ = cb;
}	

void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready = epoll_wait(epollfd_,
							&(*eventsList_.begin()),
							eventsList_.size(),
							5000);
	}while (-1 == nready && errno == EINTR);
	
	if (-1 == nready)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if (0 == nready)
		printf("epoll_wait timeout\n");
	else
	{
		if (nready == static_cast<int>(eventsList_.size()))
			eventsList_.resize(eventsList_.size() * 2);
		
		for (int idx = 0; idx != nready; ++idx)
			if (eventsList_[idx].events & EPOLLIN)	
			{
				if (eventsList_[idx].data.fd == listenfd_)
					handleConnection();
				else handleMessage(eventsList_[idx].data.fd);
			}
	}
}

void EpollPoller::handleConnection()
{
	int peerfd = acceptConnFd(listenfd_);
	addEpollFdRead(epollfd_, peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd));	
	conn->setConnectionCallback(onConnectionCb_);
	conn->setMessageCallback(onMessageCb_);
	conn->setCloseCallback(onCloseCb_);

	pair<ConnectionMap::iterator, bool> ret;
	ret = connMap_.insert(make_pair(peerfd, conn));
	assert(ret.second == true);
	
	conn->handleConnectionCallback();
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);
	ConnectionMap::iterator it = connMap_.find(peerfd);
	assert(it != connMap_.end());

	if (isClosed)
	{
		it->second->handleCloseCallback();
		delEpollFdRead(epollfd_, peerfd);
		connMap_.erase(it);
	}
	else it->second->handleMessageCallback();
}
