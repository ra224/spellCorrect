#ifndef __CACHE_H
#define __CACHE_H

#include "MutexLock.h"
#include <ext/hash_map>
#include <string>

using namespace std;
using namespace __gnu_cxx;

struct MyHashFn
{
	size_t operator()(const string &str) const
	{
		return __stl_hash_string(str.c_str());
	}
};

class Cache
{
public:
	typedef hash_map<string, string, MyHashFn> HashMap;
	typedef hash_map<string, string, MyHashFn>::iterator HashMap_it;
	typedef hash_map<string, string, MyHashFn>::const_iterator HashMap_cit;

	Cache(const int num = 100);
	Cache(const Cache &cache);

	void map_to_cache(string &key, string &value);

	string query_cache(const string &word);

	void write_to_file(ofstream &fout);

	void read_from_file(const string &filename);

	void add_elements(const Cache &cache);
private:
	HashMap hashmap_;
	MutexLock mutex_;
};

#endif
