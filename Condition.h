#ifndef _CONDITION_H
#define _CONDITION_H

#include "Noncopyable.h"
#include "MutexLock.h"

class Condition : Noncopyable
{
public:
	Condition(MutexLock & mutex);
	~Condition();

	void wait();
	void notify();
	void notifyall();

private:
	pthread_cond_t cond_;
	MutexLock & mutex_;
};

#endif
