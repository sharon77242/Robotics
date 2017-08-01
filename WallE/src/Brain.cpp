/*
 * Brain.cpp
 *
 *  Created on: Jun 21, 2016
 *      Author: user
 */

#include "Brain.h"

Brain::Brain() {
	// TODO Auto-generated constructor stub

}

void Brain::initializeRouting(){
	Vector2d startPoint = _configurationManager->getRobotStartLocation().vector2dValue() / (map->getGridMapResolutionRatio() / 2);
	Vector2d endPoint = _configurationManager->getRobotGoalLocation().vector2dValue() / (map->getGridMapResolutionRatio() / 2);
	_pathPlanner = new PathPlanner(map, startPoint, endPoint);

	list<Vector2d> route = _pathPlanner->performAStar();
	_waypointsManager = new WaypointsManager(route, map);

	list<Vector2d> wayPoints = _waypointsManager->getWaypoints(WaypointsManager::WAYPOINT_FOR_GRID);
	cout << "The route size is: " << route.size() << ", with " << wayPoints.size() << " waypoints" << endl;

	map->markRoute(route, map->getGridResolution());
	map->markWayPoints(wayPoints, map->getGridResolution());
	map->markStartPoint(startPoint, map->getGridResolution());
	map->markGoalPoint(endPoint, map->getGridResolution());
	map->colorMap("mapWithPath.png");

	_robot->Read();
}
Brain::Brain(Robot* robot) {
	_configurationManager = ConfigurationManager::getInstance();
	_robot = robot;
}

void Brain::run() {
	initializeMap();
	initializeRouting();
	initializeLocalization();

	currRobotLocation = _configurationManager->getRobotStartLocation().vector2dValue();
	list<Vector2d> wayPoints = _waypointsManager->getWaypoints(WaypointsManager::WAYPOINT_FOR_MAP);

	for (list<Vector2d>::iterator wayPointsIterator = wayPoints.begin(); wayPointsIterator != wayPoints.end(); wayPointsIterator++) {
		Vector2d currentWayPoint = *wayPointsIterator.operator ->();
		_robot->Read();
		string final;
		stringstream ss;
		int counter = 0;
		int printMapCounter = 1;
		while (_robot->getPosition().distanceBetweenVector(currentWayPoint) > Globals::TOLERANCE) {
			if (counter == 20)
			{
				counter=0;
				ss << (int)floor(printMapCounter);
				final = "/home/user/Desktop/Maps/currentMap" + ss.str() + ".png";
				map->colorMap(final.c_str());
				printMapCounter++;
				ss.str("");
				ss.clear();
			}
			cout << "The Robot's current position is: " << _robot->getLocation().getX()  << " " << _robot->getLocation().getY() << " " << _robot->getLocation().getYaw()<< endl;
			MoveToPoint * moveToPointWheels = new MoveToPoint(_robot, _localizationManager, currentWayPoint);

			_robot->Read();
			moveToPointWheels->action();
			lastLocation = currRobotLocation;
			currRobotLocation = _robot->getPosition();
			map->markCurrentLocation((currRobotLocation/(map->getGridMapResolutionRatio() / 2)),map->getGridResolution());
			map->markBlank((lastLocation/(map->getGridMapResolutionRatio() / 2)),map->getGridResolution());

			counter++;
		}
	}

	cout << "Done!" << endl;
}

void Brain::initializeLocalization() {
	Location realRobotStartLocation(_configurationManager->getRobotStartLocation().vector2dValue(), _configurationManager->getRobotStartLocation().getYaw());
	_robot->setRobotLocation(realRobotStartLocation);

	for(int i = 0; i < 15; i++)
		_robot->Read();

	_localizationManager = new LocalizationManager(realRobotStartLocation, map);
}

void Brain::initializeMap() {
	map = new Map();
	map->getParams();
	const char* path = map->strMapPath;
	map->BlowMap(path);
	map->MapToGrid();
	map->colorMap(map->strMapPath);
}
Brain::~Brain() {
	// TODO Auto-generated destructor stub
}
