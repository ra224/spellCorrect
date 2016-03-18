#ifndef _TASK_H
#define _TASK_H

#include "Cache.h"
#include "MyDict.h"
#include <string>
#include <queue>
#include <set>

using namespace std;

struct MyResult
{
	string word_;
	int ifreq_;
	int idist_;
};

struct MyCompare
{
	bool operator()(const MyResult &lhs, const MyResult &rhs)
	{
		if (lhs.idist_ > rhs.idist_)  return true;
		if (lhs.idist_ < rhs.idist_)  return false;
		if (lhs.ifreq_ < rhs.ifreq_)  return true;
		if (lhs.ifreq_ > rhs.ifreq_)  return false;
		if (lhs.word_ > rhs.word_) return true;
		return false;
	}
};

class Task
{
public:
	Task(const string &expr, int sockfd, MyDict &mydict);
	Task(const char *expr, int sockfd, MyDict &mydict);
	void execute(Cache &cache);

private:
	void query_idx_table();
	void statistic(set<int> &iset);
	int distance(const string &rhs);
	void response(Cache &cache);

private:
	string expr_;
	int sockfd_;
	MyDict &mydict_;
	priority_queue<MyResult, vector<MyResult>, MyCompare> que_res_;
};

#endif
