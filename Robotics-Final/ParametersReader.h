#ifndef PARAMETERSREADER_H_
#define PARAMETERSREADER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "./Models/Position.h"

using namespace std;

class ParametersReader {
private:
	map<string, string> _params;
	string Trim(string str);
	
public:
	ParametersReader(const char *path);
	Position* GetStartLocation();
	Position* GetGoal();
	int GetRobotSize();
	virtual ~ParametersReader();
};

#endif /* PARAMETERSREADER_H_ */
