/*
 * ConfigurationManager.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: user
 */

#include "ConfigurationManager.h"

ConfigurationManager * ConfigurationManager::_instance = NULL;


ConfigurationManager::ConfigurationManager()
{
	readParams();

}

ConfigurationManager::~ConfigurationManager() {
	// TODO Auto-generated destructor stub
}

ConfigurationManager * ConfigurationManager::getInstance() {
	if (ConfigurationManager::_instance == NULL)
		_instance = new ConfigurationManager();

	return _instance;
}

Location ConfigurationManager::getRobotStartLocation() {
	return _robotStartLocation;
}

Location ConfigurationManager::getRobotGoalLocation() {
	return _robotGoalLocation;
}
void ConfigurationManager::setGridResolution(float gridResolution)
{
	gridResolution = gridResolution;
}
void ConfigurationManager::setMapResolution(float mapResolution)
{
	mapResolution = mapResolution;
}

float ConfigurationManager::getMapResolutionCM() {
	return _mapResolutionCM;
}

float ConfigurationManager::getGridResolutionCM() {
	return _gridResolutionCM;
}

Vector2d ConfigurationManager::getRobotSize() {
	return _robotSize;
}

char* ConfigurationManager::getMapFilePath() {
	return _mapFilePath;
}

string ConfigurationManager::trim(string str) {
	while (str.find_first_of(" ") == 0) {
		str = str.substr(1, str.length() + 1);
	}

	while (str.find_first_of(" ") == str.length() - 1) {
		str = str.substr(0, str.length());
	}

	return str;
}

vector<string> ConfigurationManager::splitString(string str, char delimiter) {
	vector<string> splittedString;
	size_t pos = 0;

	while ((pos = str.find(delimiter)) != std::string::npos) {
		splittedString.push_back(trim(str.substr(0, pos)));
		str.erase(0, pos + 1);
	}

	splittedString.push_back(trim(str));

	return splittedString;
}


void ConfigurationManager::setProperty(string key, string value) {
	if (key == "map") {
		_mapFilePath = strdup(value.c_str());
	} else if (key == "startLocation") {
		_robotStartLocation = ConvertToLocation(value);
	} else if (key == "goal") {
		_robotGoalLocation = ConvertToLocation(value);
	} else if (key == "robotSize") {
		_robotSize = ConvertToSize(value);
	} else if (key == "MapResolutionCM") {
		_mapResolutionCM = atof(value.c_str());
	} else if (key == "GridResolutionCM") {
		_gridResolutionCM = atof(value.c_str());
	}
}

string ConfigurationManager::getPropertyKey(string str) {
	return splitString(str, Globals::CONFIGURATION_DELIMITER)[0];
}

string ConfigurationManager::getPropertyValue(string str) {
	return splitString(str, Globals::CONFIGURATION_DELIMITER)[1];
}


// Reads the parameters received from the configuration file
void ConfigurationManager::readParams()
{
	string line;
	ifstream paramsfile ("parameters.txt");
	if (paramsfile.is_open())
	{
		while ( getline (paramsfile,line) )
		{
			setProperty(getPropertyKey(line), getPropertyValue(line));
	    }
	    paramsfile.close();
	}

	else cout << "Unable to open file" << endl;
}

Location ConfigurationManager::ConvertToLocation(string value) {
	vector<string> LocationVector = splitString(value, ' ');

	int x = atoi(LocationVector[0].c_str());
	int y = atoi(LocationVector[1].c_str());
	float yaw = (LocationVector.size() > 2) ? atoi(LocationVector[2].c_str()) : 0;

	return Location(x, y, yaw);
}

Vector2d ConfigurationManager::ConvertToSize(string value) {
	vector<string> SizeVector = splitString(value, ' ');

	int width = atoi(SizeVector[0].c_str());
	int height = atoi(SizeVector[1].c_str());

	return Vector2d(width, height);
}
