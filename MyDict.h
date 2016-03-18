#ifndef _MYDICT_H
#define _MYDICT_H

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class MyDict
{
public:
	static MyDict * createInstance(const char *dictpath, const char *dictpath2);
	
	vector<pair<string, int> > &get_dict();
	map<string, set<int> > &get_index_table();

	void show_dict();
	void show_index_table();

private:
	MyDict(const char *dictpath, const char *dictpath2);
	void read_from(const char * dictpath);
	void record_to_index(int idx);

private:
	static MyDict *pInstance;
	vector<pair<string, int> > dict_;
	map<string, set<int> > index_table_;
};

#endif
