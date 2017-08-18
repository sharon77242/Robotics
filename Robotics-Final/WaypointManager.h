#ifndef WAYPOINTMANAGER_H_
#define WAYPOINTMANAGER_H_

#include <vector>
#include "Models/Node.h"
#include "Models/Position.h"
#include "Map.h"

class WaypointManager
{
public:
	WaypointManager(Map* map);
	std::vector<Node*> OptimizePath(std::vector<Node*> path);

private:
	bool IsPathClear(Node * firstNode, Node * secondNode);
	bool isLineClear(int ySecond, int yFirst, int xSecond, int xFirst, int smallX, int bigX);

	Map *map_;
};

#endif /* WAYPOINTMANAGER_H_ */
