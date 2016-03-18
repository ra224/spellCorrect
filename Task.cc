#include "Task.h"
#include <iostream>
#include <unistd.h>

size_t nBytesCode(const char ch)
{
	size_t nBytes = 0;
	if (ch &(1 << 7))
	{//对中文进行处理-utf8编码
		if((ch & 0xF0) == 0xC0 || (ch & 0xF0) == 0xD0)   //  1111 0000
		{												 // &11xx xxxx
			nBytes += 2;								 //  1100 0000	
		}												 //  1101 0000
		else if((ch & 0xF0) == 0xE0)
		{
			nBytes += 3;
		}
		else if((ch & 0xF0) == 0xF0)
		{
			nBytes += 4;
		}
		else if((ch & 0xFF) == 0xF8)
		{
			nBytes += 5;
		}
		else if((ch & 0xFF) == 0xFE)
		{
			nBytes += 6;
		}
	}
	else
	{//1字节编码或英文
		nBytes += 1;	
	}
	return nBytes;
}

size_t length(const string &str)
{
	size_t ilen = 0;
	for (size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += nBytes - 1;
		++ilen;
	}
	return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

Task::Task(const string & expr, int sockfd, MyDict &mydict)
	:	expr_(expr),
		sockfd_(sockfd),
		mydict_(mydict)
{
}


Task::Task(const char *expr, int sockfd, MyDict &mydict)
	:	expr_(expr),
		sockfd_(sockfd),
		mydict_(mydict)
{
}

void Task::execute(Cache &cache)
{
	string str = cache.query_cache(expr_);
	if (str != string() )
	{
		int nwrite = write(sockfd_, str.c_str(), str.size());
		if (-1 == nwrite)
			cout << "response error" << endl;
		cout << "from cache response client: " << str << endl;
		return;
	}

	query_idx_table();
	response(cache);
}

void Task::query_idx_table()
{
	string ch;
	auto idx_table = mydict_.get_index_table();
	for (size_t idx = 0; idx != expr_.size(); ++idx)
	{
		size_t nBytes = nBytesCode(expr_[idx]);
		ch = expr_.substr(idx, nBytes);
		cout << ch << endl;
		idx += nBytes - 1;
		if (idx_table.count(ch))
		{
			cout << "idx_table count() return true" << endl;
			statistic(idx_table[ch]);
		}
		/*else 
		{
			fprintf(stderr, "no such letter!");
			exit(EXIT_FAILURE);
		}*/
	}
}

void Task::statistic(set<int> &iset)
{
	auto dict = mydict_.get_dict();
	auto iter = iset.begin();
	for (; iter != iset.end(); ++iter)
	{
		string rhs_word = dict[*iter].first;
	//	cout << rhs_word << endl;
		int idist = distance(rhs_word);
		if (idist < 3)
		{
			MyResult res;
			res.word_ = rhs_word;
			res.idist_ = idist;
			res.ifreq_ = dict[*iter].second;
			que_res_.push(res);
			cout << res.word_ << ' ' << res.idist_ << ' ' << res.ifreq_ << endl;
		}
	}
}

int Task::distance(const string &rhs)
{
	size_t lhs_len = length(expr_);
	size_t rhs_len = length(rhs);

	int editDist[lhs_len + 1][rhs_len + 1];
	for (size_t idx = 0; idx <= lhs_len; ++idx)
	{
		editDist[idx][0] = idx;
	}
	
	for (size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}

 	string sublhs, subrhs;
	for (size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)
	{
		size_t nBytes = nBytesCode(expr_[lhs_idx]);
		sublhs = expr_.substr(lhs_idx, nBytes);
		lhs_idx += nBytes - 1;
		for (size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += nBytes - 1;
			if (sublhs == subrhs)
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];
			}
			else
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);	
			}
		}
	}
	
	return editDist[lhs_len][rhs_len];
}

void Task::response(Cache &cache)
{
	if (que_res_.empty())
	{
		string res = "no answer!";
		int nwrite = write(sockfd_, res.c_str(), res.size());
		if (-1 == nwrite)
			cout << "response error" << endl;
		cout << "response client" << endl;
	}
	else
	{
		MyResult res = que_res_.top();
		int nwrite = write(sockfd_, res.word_.c_str(), res.word_.size());
		if (-1 == nwrite)
			cout << "response error" << endl;
		cout << "response client" << endl;
		cache.map_to_cache(expr_, res.word_);
	}
}
