#include "ConfigurationManager.h"

#include "stdlib.h"

ConfigurationManager::ConfigurationManager(const string& path) {
	ifstream file(path.c_str());
	while(file)
	{
		// Split configuration file to parameter name and parameter value
		string line;
		getline(file, line);
		istringstream lineStream(line);
		string parameterName;
		getline(lineStream, parameterName, ':');
		string parameterValue;
		getline(lineStream, parameterValue, ':');

		parameterName = Trim(parameterName);
		parameterValue = Trim(parameterValue);

		_params[parameterName] = parameterValue;
	}

	file.close();

}

string ConfigurationManager::Trim(string str)
{
	// Trim start
	while(str.length() &&
			(str.at(0) == ' ' || str.at(0) == 't' ||
					str.at(0) == '\r' || str.at(0) == '\n'))
	{
		str.erase(0, 1);
	}

	// Trim end
	while(str.length() &&
			(str.at(str.length()-1) == ' ' || str.at(str.length()-1) == '\t' ||
					str.at(str.length()-1) == '\r' || str.at(str.length()-1) == '\n'))
	{
		str.erase(str.length()-1, 1);
	}

	return str;
}

Position ConfigurationManager::GetStartLocation()
{
	string valueStr = _params["startLocation"];
	istringstream lineStream(valueStr);
	string x;
	getline(lineStream, x, ' ');
	string y;
	getline(lineStream, y, ' ');
	string yaw;
	getline(lineStream, yaw, ' ');

	Trim(x);
	Trim(y);
	Trim(yaw);

	return Position (atof(x.c_str()), atof(y.c_str()), atof(yaw.c_str()));
}

Position* ConfigurationManager::GetGoal()
{
	string valueStr = _params["goal"];
	istringstream lineStream(valueStr);
	string x;
	getline(lineStream, x, ' ');
	string y;
	getline(lineStream, y, ' ');

	Trim(x);
	Trim(y);

	Position *goalPos = new Position(atof(x.c_str()), atof(y.c_str()));
	return goalPos;
}

int ConfigurationManager::GetRobotSize()
{
	string valueStr = _params["robotSize"];
	istringstream lineStream(valueStr);
	Trim(valueStr);

	return atoi(valueStr.c_str());
}

ConfigurationManager::~ConfigurationManager() {
}

