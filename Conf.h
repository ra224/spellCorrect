#ifndef _CONF_H
#define _CONF_H

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

using namespace std;

class MyConf
{
public:
	MyConf(string &);

	bool init();

	map<string, string> & getMap()
	{
		return map_;
	}

	void show();
private:
	ifstream in_;
	map<string, string> map_;
};

#endif
