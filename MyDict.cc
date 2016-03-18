#include "MyDict.h"
#include <iostream>
#include <fstream>
#include <sstream>

MyDict *MyDict::pInstance = NULL;

MyDict *MyDict::createInstance(const char * dictpath, const char * dictpath2)
{
	if (NULL == pInstance)
		pInstance = new MyDict(dictpath, dictpath2);
	return pInstance;
}

vector<pair<string, int> > &MyDict::get_dict()
{
	return dict_;
}

map<string, set<int> > &MyDict::get_index_table()
{
	return index_table_;
}

void MyDict::show_dict()
{
	for (auto iter = dict_.begin(); iter != dict_.end(); ++iter)
		cout << iter->first << "-->" << iter->second << endl;
}

void MyDict::show_index_table()
{
	for (auto iter = index_table_.begin(); iter!=index_table_.end(); ++iter)
	{
		cout << iter->first << "-->";
		for (auto sit = iter->second.begin(); sit != iter->second.end(); ++sit)
			cout << *sit << " ";
		cout << endl;
	} 
}

void MyDict::read_from(const char * dictpath)
{
	ifstream in(dictpath);
	if (!in)
	{
		cout << __DATE__ << " " << __TIME__
  			 << __FILE__ << " " << __LINE__
			 << ": dict read error" << endl;
		exit(-1);	
	}
		
	string line;
	while (getline(in,line))
	{
		stringstream ss(line);
		string key;
		int value;
		ss >> key >> value;
		dict_.push_back(make_pair(key,value));
	}
	in.close();
}

MyDict::MyDict(const char *dictpath, const char * dictpath2)
{
	read_from(dictpath);
	read_from(dictpath2);
	for (size_t idx = 0; idx != dict_.size(); ++idx)
	{
		record_to_index(idx);
	}
}

void MyDict::record_to_index(int idx)
{
	string key;
	string word = dict_[idx].first;
//	cout << word << endl;
	for (size_t iidx = 0; iidx != word.size(); ++iidx)
	{
		char ch = word[iidx];
		if (ch & (1 << 7))
		{
			if ( (ch & 0XF0) == 0XC0 || (ch & 0xF0) == 0xD0 )
			{
				key = word.substr(iidx, 2);
				++iidx;
			}
			else if ((ch & 0xF0) == 0xE0)
			{
				key = word.substr(iidx, 3);
				iidx += 2;
			}
			else if ((ch & 0xF0) == 0xF0)
			{
				key = word.substr(iidx, 4);
				iidx += 3;
			}
			else if ((ch & 0xFF) == 0xF8)
			{
				key = word.substr(iidx, 5);
				iidx += 4;
			}
			else if ((ch & 0xFF) == 0xFE)
			{
				key = word.substr(iidx, 6);
				iidx += 5;
			}
		}
		else 
		{
			key = word.substr(iidx, 1);
		}
		index_table_[key].insert(idx);
	}
}
