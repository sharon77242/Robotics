/*
 * ConfigurationManger.h
 *
 *  Created on: Aug 1, 2017
 *      Author: user
 */

#ifndef CONFIGURATIONMANAGER_H_
#define CONFIGURATIONMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <HamsterAPIClientCPP/Hamster.h>
#include "Declerations.h"
#include "Position.h"

using namespace HamsterAPI;
using namespace std;

class ConfigurationManager
{
public:
	static void Init()
	{
		Position startPos;
		startPos.x = 0;
		startPos.y = 0;
		startPos.yaw = 0;
		instance = new ConfigurationManager(startPos, 5, 0, 0.25, 0.05);
	}

	static void Init(string fileName)
	{
		ifstream file(fileName.c_str());
		if(!file.is_open()) return;

		string value;
		float params[7];
		int i = 0;
		while (file.good() && i < 6)
		{
			getline(file, value, ',');
			params[i] = atof(value.c_str());
			i++;
		}

		getline(file, value, '\n');
		params[i] = atof(value.c_str());

		file.close();

		Position position;
		position.x = params[0];
		position.y = params[1];
		position.yaw  = params[2];
		double pointX = params[3];
		double pointY = params[4];
		double robotSize = params[5];
		double mapResolution = params[6];

		instance = new ConfigurationManager(position, pointX, pointY, robotSize, mapResolution);
	}

	static void PrintData()
	{
		cout << "Start Position (X, Y, Yaw): " << instance->startPosition.x << ", " << instance->startPosition.y  << ", " << instance->startPosition.yaw << endl;
		cout << "Destination Position (X, Y): " << instance->destPointX << ", " << instance->destPointY << endl;
		cout << "Robot Size: " << instance->robotSize << endl;
		cout << "Map Resolution: " << instance->mapResolution << endl;
	}

	static Position getStartPosition()
	{
		return instance->startPosition;
	}

	static double getRobotSize()
	{
		return instance->robotSize;
	}

	static double getDestPointX()
	{
		return instance->destPointX;
	}

	static double getDestPointY()
	{
		return instance->destPointY;
	}

	static double getMapResolution()
	{
		return instance->mapResolution;
	}

	static void Destroy()
	{
		if (instance)
		{
			delete instance;
		}
	}
private:
	static ConfigurationManager *instance;

	Position startPosition;
	double destPointX;
	double destPointY;
	double robotSize;
	double mapResolution;

	ConfigurationManager(Position startPosition, double destPointX, double destPointY, double robotSize,double mapResolution)
		: startPosition(startPosition), destPointX(destPointX), destPointY(destPointY), robotSize(robotSize), mapResolution(mapResolution)
	{
	}
};

ConfigurationManager *ConfigurationManager::instance = 0;

#endif /* CONFIGURATIONMANAGER_H_ */
