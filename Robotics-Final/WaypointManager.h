#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_

#include <vector>
#include "Models/Node.h"
#include "Models/Position.h"
#include "Map.h"

class WaypointManager
{
public:
	WaypointManager(const HamsterAPI::OccupancyGrid& inflatedGrid);
	std::vector<Node*> GetWaypoints(std::vector<Node*> path);

private:
	int getNextWaypoint(vector<Node*> path, int startNodeIndex);
	bool raytraceWillCollide(Node* start, Node* end);

	const HamsterAPI::OccupancyGrid inflatedGrid_;
};

#endif /* WAYPOINTMANAGER_H_ */
