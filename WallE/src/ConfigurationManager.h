/*
 * ConfigurationManager.h
 *
 *  Created on: Jun 14, 2016
 *      Author: user
 */

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include "pngUtil.h"
#include "lodepng.h"
#include "Location.h"
#include "Globals.h"

using namespace std;

class ConfigurationManager {
private:
	static ConfigurationManager * _instance;
	vector<string> data;
	char* _mapFilePath;
	Location _robotStartLocation;
	Location _robotGoalLocation;
	Vector2d _robotSize;
	float _mapResolutionCM;
	float _gridResolutionCM;
	string trim(string str);
	vector<string> splitString(string str, char delimiter);
	void setGridResolution(float gridResolution);
	void setMapResolution(float mapResolution);
	Location ConvertToLocation(string value);
	Vector2d ConvertToSize(string value);
	string getPropertyKey(string s);
	string getPropertyValue(string s);
	void setProperty(string key, string value);

public:
	vector<string> params;
	//string line;
	ConfigurationManager();
	virtual ~ConfigurationManager();

	// splits the received string by the delimiter into vector
	void split(const string &s, char delim);

	// Reads the parameters received from the configuration file
	void readParams();

	static ConfigurationManager * getInstance();
	float getMapResolutionCM();
	float getGridResolutionCM();
	Location getRobotStartLocation();
	Location getRobotGoalLocation();
	Vector2d getRobotSize();
	char* getMapFilePath();
};

#endif /* CONFIGURATIONMANAGER_H_ */
