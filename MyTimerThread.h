#ifndef __MYTIMERTHREAD_H
#define __MYTIMERTHREAD_H

#include "Timer.h"
#include "TimerThread.h"

using namespace std;

class MyTimerThread
{
public:
	typedef function<void ()> Callback;

	MyTimerThread(int val, int interval, Callback cb);

	void start();
	void stop();

private:
	Timer timer_;
	TimerThread thread_;
};

#endif
