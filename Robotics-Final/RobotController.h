#ifndef ROBOTCONTROLLER_H_
#define ROBOTCONTROLLER_H_

#include "Map.h"
#include "RobotMovement.h"
#include "math.h"
#include "WaypointManager.h"
#include "AStarAlgo.h"
#include "RobotController.h"
#include "ParametersReader.h"

class RobotController
{

private:
	Hamster* _hamster;
	ParametersReader* _config;
	Map* _map;
	OccupancyGrid _occupancygrid;
	ParticlesManager* _particlesManager;
	RobotMovement* _robotMovement;
	WaypointManager* _waypointsManager;

	vector<Node*> CreatePathForMovement();
	vector<Node*> CreateWayPoints(vector<Node*> path);
	void MoveRobotToWayPoint(Node* node);


public:
	RobotController(Hamster* hamster, ParametersReader* config);
	void Start();
};


#endif
