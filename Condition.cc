#include "Condition.h"
#include <assert.h>

Condition::Condition(MutexLock & mutex)
	: mutex_(mutex)
{
	TINY_CHECK(!pthread_cond_init(&cond_,NULL));	
}

Condition::~Condition()
{
	TINY_CHECK(!pthread_cond_destroy(&cond_));
}

void Condition::wait()
{
	assert(mutex_.isLocking());
	TINY_CHECK(!pthread_cond_wait(&cond_ , mutex_.getMutexLockPtr()));
	mutex_.restoreMutexStatus();
}

void Condition::notify()
{
	TINY_CHECK(!pthread_cond_signal(&cond_));
}

void Condition::notifyall()
{
	TINY_CHECK(!pthread_cond_broadcast(&cond_));
}
