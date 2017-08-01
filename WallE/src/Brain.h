/*
 * Brain.h
 *
 *  Created on: Jun 21, 2016
 *      Author: user
 */

#ifndef BRAIN_H_
#define BRAIN_H_

#include <list>
#include <iostream>

#include "Robot.h"
#include "Map.h"
#include "LocalizationManager.h"
#include "PathPlanner.h"
#include "WaypointsManager.h"
#include "Vector2d.h"
#include "Robot.h"

class Brain {
private:
	Robot * _robot;
	ConfigurationManager * _configurationManager;
	Map * map;
	PathPlanner * _pathPlanner;
	WaypointsManager * _waypointsManager;
	LocalizationManager * _localizationManager;
	Vector2d lastLocation;
	Vector2d currRobotLocation;

	void initializeMap();
	void initializeRouting();
	void initializeLocalization();
	list<Vector2d> getRoute();

public:
	Brain();
	Brain(Robot * robot);
	virtual ~Brain();

	void run();
};

#endif /* BRAIN_H_ */
