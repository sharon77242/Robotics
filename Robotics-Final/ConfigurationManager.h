#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "./Models/Position.h"

using namespace std;

class ConfigurationManager {
private:
	map<string, string> _params;
	string Trim(string str);
	
public:
	ConfigurationManager(const string& path);
	Position GetStartLocation();
	Position* GetGoal();
	double GetRobotSize();
	virtual ~ConfigurationManager();
};

#endif /* CONFIGURATIONMANAGER_H_ */
