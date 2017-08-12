#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_

#define WAYPOINT_MAX_DISTANCE 20

#include <vector>
#include "Models/Node.h"
#include "Models/Position.h"
#include "Map.h"

using namespace std;

class WaypointManager {

private:
	bool IsPathClear(Node * firstNode, Node * secondNode);
	Map *_map;

public:
	WaypointManager(Map * map);
	virtual ~WaypointManager();
	vector<Node*> OptimizePath(vector<Node*> path);
};

#endif /* WAYPOINTMANAGER_H_ */
