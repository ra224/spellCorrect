#include "Thread.h"
#include <assert.h>


Thread::Thread(ThreadCallback cb, Cache &cache)
	: cb_ (cb),
      isRunning_(false),
	  pthId_(0),
	  cache_(cache)	
{
	cout << "Thread::Thread(ThreadCallback cb)" << endl;
}

Thread::~Thread()
{
	if (isRunning_)
		TINY_CHECK(!pthread_detach(pthId_));
	cout << "Thread::~Thread()" << endl;
}	

void Thread::start()
{
	TINY_CHECK(!pthread_create(&pthId_, NULL , threadFunc , (void*)this));
	isRunning_ = true;
}

void Thread::join()
{
	assert(isRunning_);
	TINY_CHECK(!pthread_join(pthId_ , NULL));
	isRunning_ = false;	
}

Cache &Thread::get_cache()
{
	return cache_;
}

void * Thread::threadFunc(void * arg)
{
	Thread * pThread = static_cast<Thread*>(arg);
	if (pThread != NULL)
	{
		pThread->cb_(pThread->cache_);
		return pThread;
	}
	return NULL;
}
