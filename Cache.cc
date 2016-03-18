#include "Cache.h"
#include <iostream>
#include <fstream>

Cache::Cache(const int num)
	: hashmap_(num), mutex_()
{
}

Cache::Cache(const Cache &cache)
	: hashmap_(cache.hashmap_),
	  mutex_()
{
}

void Cache::map_to_cache(string &key, string &value)
{
	MutexLockGuard guard(mutex_);
	hashmap_[key] = value;
}

string Cache::query_cache(const string &word)
{
	HashMap_it iter = hashmap_.find(word);
	if (iter != hashmap_.end())
		return iter->second;
	else
    	return string();
}

void Cache::write_to_file(ofstream &fout)
{
	MutexLockGuard guard(mutex_);
	HashMap_it iter;
	for (iter = hashmap_.begin(); iter != hashmap_.end(); ++iter)
		fout << iter->first << "\t" << iter->second << endl;
}	

void Cache::read_from_file(const string &filename)
{
	ifstream fin(filename.c_str());
	if (!fin)
	{
		cout << "open cache file failed!\n" << endl;
		return;
	}

	string key, value;
	while (fin >> key >> value)
	{
		hashmap_.insert(make_pair(key, value));
		cout << key << " " << value << endl;
	}	
}

void Cache::add_elements(const Cache &cache)
{
	MutexLockGuard guard(mutex_);
	HashMap_cit hit = cache.hashmap_.begin();
	for (; hit != cache.hashmap_.end(); ++hit)
		hashmap_.insert(*hit);
}
