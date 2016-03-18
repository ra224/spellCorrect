#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "Thread.h"
#include "Condition.h"
#include "MutexLock.h"
#include "Task.h"
#include "Cache.h"
#include <vector>
#include <queue>
#include <functional>
#include <memory>

using namespace std;

class Threadpool : Noncopyable
{
public:
	Threadpool(size_t queueSize , size_t threadsNum, Cache &cache, MyDict &mydict);
	
	void start();

	void addTask(Task task);
	Task getTask();

	void runInThread(Cache &cache);

	void update();

private:
	mutable MutexLock mutex_;
	Condition notEmpty_;
	Condition notFull_;
	
	size_t queueSize_;
	queue<Task> queue_;	

	const size_t threadsNum_;
	vector<unique_ptr<Thread> > threads_;
	bool isExit_;

	Cache &cache_;

public:
	MyDict &mydict_;
};

#endif
