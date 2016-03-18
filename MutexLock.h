#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include "Noncopyable.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TINY_CHECK(exp)	\
	if (!(exp))	\
	{	\
		fprintf(stderr,"File:%s, Line:%d Exp:[" #exp "] is true, abort.\n",__FILE__,__LINE__); abort();\
	}

class MutexLock : Noncopyable
{
	friend class Condition;

public:
	MutexLock();
	~MutexLock();	

	void lock();
	void unlock();

	bool isLocking() const { return isLocking_; }
	pthread_mutex_t * getMutexLockPtr()
	{ return &mutex_; }

private:
	void restoreMutexStatus()
	{
		isLocking_ = true;
	}
	
	pthread_mutex_t mutex_;
	bool isLocking_;
};

class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex);
	~MutexLockGuard();

private:
	MutexLock & mutex_;
};

#endif
