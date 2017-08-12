#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Map.h"
#include "RobotMovement.h"
#include "math.h"
#include "WaypointManager.h"
#include "RobotController.h"

#include "ConfigurationManager.h"
#include "PathPlanner.h"

class RobotController
{

private:
	Hamster& _hamster;
	ConfigurationManager* _config;
	Map* _map;
	OccupancyGrid _occupancygrid;
	ParticlesManager* _particlesManager;
	RobotMovement* _robotMovement;
	WaypointManager* _waypointsManager;

	vector<Node*> CreatePathForMovement();
	vector<Node*> CreateWayPoints(vector<Node*> path);
	void MoveRobotToWayPoint(Node* node);


public:
	RobotController(Hamster& hamster, ConfigurationManager* config);
	void Start();
};


#endif
