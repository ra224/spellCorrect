#include "Conf.h"

MyConf::MyConf(string & path)
	:in_(path.c_str())
{
}

bool MyConf::init()
{
	if (!in_)
	{
		cout << "file read error" << endl;
		return false;
	}
	string line;
	while (getline(in_, line))
	{
		istringstream iss(line);
		string key, value;
		iss >> key >> value;
		map_.insert(make_pair(key, value));
	}
	in_.close();
	return true;
}

void MyConf::show()
{
	map<string, string>::const_iterator mit = map_.begin();
	for (; mit != map_.end(); ++mit)
	{
		cout << mit->first << " --> " << mit->second << endl;
	}
}
