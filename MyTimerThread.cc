#include "MyTimerThread.h"

MyTimerThread::MyTimerThread(int val, int interval, Callback cb)
	:	timer_(val, interval, cb),
		thread_(bind(&Timer::start, &timer_))
{
}

void MyTimerThread::start()
{
	thread_.start();
}

void MyTimerThread::stop()
{
	timer_.stop();
	thread_.join();
}
