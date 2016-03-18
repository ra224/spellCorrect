#include "TimerThread.h"

TimerThread::TimerThread(Callback cb)
	:	pthid_(0),
		isRunning_(false),
		cb_(cb)
{
}

TimerThread::~TimerThread()
{
	if (isRunning_)
		pthread_detach(pthid_);
}

void TimerThread::start()
{
	pthread_create(&pthid_, NULL, &TimerThread::runInThread, this);
	isRunning_ = true;
}

void TimerThread::join()
{
	pthread_join(pthid_, NULL);
	isRunning_ = false;
}

void *TimerThread::runInThread(void * arg)
{
	TimerThread *ptt = static_cast<TimerThread*>(arg);
	if (ptt->cb_)
		ptt->cb_();
	return NULL;
}
