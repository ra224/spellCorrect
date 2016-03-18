#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include <functional>
#include <pthread.h>
#include "Noncopyable.h"

using namespace std;

class TimerThread : Noncopyable
{
public:
	typedef function<void ()> Callback;

	explicit TimerThread(Callback cb);
	~TimerThread();

	void start();
	void join();

private:
	static void *runInThread(void *arg);

private:
	pthread_t pthid_;
	bool isRunning_;
	Callback cb_;
};

#endif
