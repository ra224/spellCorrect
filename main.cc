#include "Conf.h"
#include "MyDict.h"
#include "Cache.h"
#include "Threadpool.h"
#include "MyTimerThread.h"
#include "InetAddress.h"
#include "TcpServer.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

static Threadpool * pThreadpool = NULL;

void onConnection(const TcpConnectionPtr & conn);
void onMessage(const TcpConnectionPtr & conn);
void onClose(const TcpConnectionPtr & conn);


int main()
{
	string filepath = "/home/ra224/spellCorrect/conf/my.conf";
	//string filepath = "/home/ra224/spellCorrect/conf/my1.conf";
	MyConf myconf(filepath);
	bool initFlag = myconf.init();
	if (initFlag)
		myconf.show();

	map<string, string> &myMap = myconf.getMap();
	string dictpath = myMap["my_dict"];
	string dictpath2 = myMap["my_cn_dict"];
	MyDict *pMydict = MyDict::createInstance(dictpath.c_str(), dictpath2.c_str());

	string cachepath = myMap["my_cache"];
	Cache cache;
	cache.read_from_file(cachepath.c_str());

	Threadpool thp(50, 10, cache, (*pMydict));	
	thp.start();
	pThreadpool = &thp;

	MyTimerThread myTmThread(1, 100, bind(&Threadpool::update, &thp));
	myTmThread.start();

	string sip = myMap["my_ip"];
	string sport = myMap["my_port"];

	int iport = 0;
	stringstream ss(sport);
	ss >> iport;

	InetAddress inetAddr(sip.c_str(), iport);
	TcpServer server(inetAddr);
	server.setConnectionCallback(&onConnection);
	server.setMessageCallback(&onMessage);
	server.setCloseCallback(&onClose);
	server.start();

	return 0;
}

void onConnection(const TcpConnectionPtr & conn)
{
	printf("%s has connected.\n", conn->toString().c_str());
	conn->send("hello, welcome to chat server.\n");
}

void onMessage(const TcpConnectionPtr & conn)
{
	string msg(conn->receive());
	string::size_type n = msg.find('\n');
	if (n != string::npos)
		msg = msg.substr(0, n);
	cout << "receive: " << msg << ",sizeof(msg) = " << msg.size() << endl;
	Task task(msg, conn->fd(), pThreadpool->mydict_);
	pThreadpool->addTask(task);
}

void onClose(const TcpConnectionPtr & conn)
{
	printf("close connection: %s.\n", conn->toString().c_str());
}
