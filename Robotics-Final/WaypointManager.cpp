#include "WaypointManager.h"

using namespace std;

WaypointManager::WaypointManager(Map *map) {
	_map = map;
}

WaypointManager::~WaypointManager() {
}

vector<Node*> WaypointManager::OptimizePath(vector<Node*> path)
{
	bool hasChanged = true;

	while(hasChanged)
	{
		hasChanged = false;

		vector<Node*> optimizedPath;
		unsigned int i;

		for(i=0; i<path.size()-2;i++)
		{
			Node* node1 = path[i];
			Node* node3 = path[i+2];

			optimizedPath.push_back(node1);

			bool isClear = IsPathClear(node1,node3);

			if(isClear)
			{
				i++;
				hasChanged = true;
			}
		}

		if(i==path.size()-2)
		{
			optimizedPath.push_back(path[path.size()-2]);
		}

		optimizedPath.push_back(path[path.size()-1]);
		path = optimizedPath;
	}

	return path;
}

bool WaypointManager::IsPathClear(Node * firstNode, Node * secondNode)
{
	bool isClear = true;

	int xFirst = firstNode->location.X();
	int xSecond = secondNode->location.X();
	int yFirst = firstNode->location.Y();
	int ySecond = secondNode->location.Y();
	int distance = sqrt(pow(xFirst-xSecond,2) + pow(yFirst-ySecond,2));

	if(distance > WAYPOINT_MAX_DISTANCE)
	{
		return false;
	}

	int bigX,smallX;

	if(xFirst > xSecond)
	{
		bigX 	= xFirst;
		smallX 	= xSecond;
	}
	else
	{
		bigX 	= xSecond;
		smallX 	= xFirst;
	}

	float a = (float)(ySecond - yFirst) / (xSecond - xFirst);

	for(double i = smallX; (i < bigX && isClear); i+=0.2)
	{
		int y = ((float)a*i) -((float)a*xFirst) + yFirst;

		if(_map->IsInflatedOccupied(i, y))
		{
			isClear = false;
		}
	}

	return isClear;
}
