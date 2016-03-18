#ifndef __TIMER_H__
#define __TIMER_H__

#include "Noncopyable.h"
#include <functional>

using namespace std;

class Timer : Noncopyable
{
public:
	typedef function<void ()> Callback;
	Timer(int val, int interval, Callback cb);
	~Timer();
		
	void start();
	void stop();

private:
	int timerfd_;
	int val_;
	int interval_;
	Callback cb_;
	bool isStarted_;
};

#endif
