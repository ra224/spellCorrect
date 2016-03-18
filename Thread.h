#ifndef _THREAD_H
#define _THREAD_H

#include "Noncopyable.h"
#include "Cache.h"
#include <pthread.h>
#include <functional>
#include <iostream>

using namespace std;

class Thread : Noncopyable
{
public:
	typedef function<void (Cache &)> ThreadCallback;

	explicit Thread(ThreadCallback cb, Cache &cache);
	~Thread();

	void start();
	void join();
	
	Cache & get_cache();

private:
	static void * threadFunc(void * arg);

private:
	pthread_t pthId_;
	bool isRunning_;
	ThreadCallback cb_;
	Cache cache_;
};

#endif

