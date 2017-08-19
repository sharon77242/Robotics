#include "RobotController.h"
#include "math.h"

RobotController::RobotController(Hamster& hamster, ConfigurationManager& config)
:_hamster(hamster)
, _config(config)
{
	_occupancygrid = hamster.getSLAMMap();

	_map = new Map(_occupancygrid, _config.GetRobotSize(), _occupancygrid.getResolution());

	_particlesManager = new LocalizationManager(hamster, _map);

	_robotMovement = new RobotMovement(
			_map,
			_particlesManager,
			_map->ConevrtMapPositionToGlobalPosition(_config.GetStartLocation()),
			MovementPose);

	_waypointsManager = new WaypointManager(_map);
}

void RobotController::Start(){
	vector<Node*> waypoints = CreateWayPoints(CreatePathForMovement());

	if(!waypoints.empty())
	{
		_map->DrawPath(waypoints);

		while(_hamster.isConnected())
		{
			for (std::vector<Node*>::iterator it = waypoints.begin() ; it != waypoints.end(); ++it)
			{
				MoveRobotToWayPoint(*it);
			}

			break;
		}
	}
}

vector<Node*> RobotController::CreatePathForMovement()
{
	Position startingPoint = _config.GetStartLocation();
	Position* goalPoint = _config.GetGoal();
	PathPlanner pp(_map);

	return pp.AStar(startingPoint, goalPoint);
}

vector<Node*> RobotController::CreateWayPoints(vector<Node*> path)
{
	if (path.empty())
	{
		cout << "No path was found!" << endl;

		return vector<Node*>();
	}

	vector<Node*> waypoints = _waypointsManager->OptimizePath(path);

	return waypoints;
}

void RobotController::MoveRobotToWayPoint(Node* node)
{
	bool arrived = false;
	Position p = _map->ConevrtMapPositionToGlobalPosition(node->location);

	sleep(1);

	while (!arrived)
	{
		arrived = _robotMovement->MoveRobotToWaypoint(_hamster,p);
	}
}

