#include "Threadpool.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Threadpool::Threadpool(size_t queueSize, size_t threadsNum, Cache &cache, MyDict &mydict)
	:	notEmpty_(mutex_),
		notFull_(mutex_),
		queueSize_(queueSize),
		threadsNum_(threadsNum),
		isExit_(false),
		cache_(cache),
		mydict_(mydict)
{
}


void Threadpool::start()
{
		for (size_t idx = 0 ; idx != threadsNum_; ++idx)
		{
				threads_.push_back(
					unique_ptr<Thread>{
						new Thread(
							bind(&Threadpool::runInThread, this, cache_),
							cache_
						)
					}
				);
		}
		for (size_t ix = 0; ix != threadsNum_; ++ix)
			threads_[ix]->start();
}

/*
void Threadpool::stop()
{
		if (!isExit_)
		{
				isExit_ = true;
				//		cout << isExit_ << endl;
				notEmpty_.notifyall();
				vector<Thread *>::iterator it;
				for (it = vecThreads_.begin(); it != vecThreads_.end(); ++it)
				{
						(*it)->join();	
						delete (*it);
				}
				vecThreads_.clear();
		}
}
*/

void Threadpool::addTask(Task task)
{
		MutexLockGuard lock(mutex_);
		while (queue_.size() >= queueSize_)
				notFull_.wait();

		queue_.push(task);
		notEmpty_.notify();
}

Task Threadpool::getTask()
{
		MutexLockGuard lock(mutex_);
/*		while ((!isExit_) && (queue_.size() == 0))
				notEmpty_.wait();
		if (!isExit_)
		{
				Task task = queue_.front();
				queue_.pop();
				notFull_.notify();
				return task;
		}
		else 
				return NULL;		*/
		while (queue_.empty())
			notEmpty_.wait();
		Task task = queue_.front();
		queue_.pop();
		notFull_.notify();
		return task;
}

void Threadpool::runInThread(Cache& cache)
{
		while (!isExit_)
		{
				Task task = getTask();
				task.execute(cache);
		}	
		//	cout << "void Threadpool::threadFunc() has exited" << endl;
}

void Threadpool::update()
{
	cout << "timer executing, update caches" << endl;
	
	for (size_t idx = 0; idx != threadsNum_; ++idx)
		cache_.add_elements(threads_[idx]->get_cache());
	
	for (size_t idx = 0; idx != threadsNum_; ++idx)
		threads_[idx]->get_cache().add_elements(cache_);
	
	cout << "cache_.write_to_file()" << endl;
	string cachepath = "/home/ra224/spellCorrect/data/cache.dat";

  	ofstream ofs(cachepath.c_str());
	cache_.write_to_file(ofs);
}
