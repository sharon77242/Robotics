#include "WaypointManager.h"

using namespace std;

WaypointManager::WaypointManager(Map *map)
: map_(map)
{}

vector<Node*> WaypointManager::OptimizePath(vector<Node*> path)
{
	bool hasChanged = true;

	while(hasChanged)
	{
		hasChanged = false;

		vector<Node*> optimizedPath;
		unsigned int i;
		unsigned int nodeIndexBeforeLast = path.size() - 2;
		for(i = 0; i < nodeIndexBeforeLast; ++i)
		{
			Node* node1 = path[i];
			Node* node3 = path[i+2];

			optimizedPath.push_back(node1);

			bool isClear = IsPathClear(node1,node3);

			if(isClear)
			{
				++i;
				hasChanged = true;
			}
		}

		if(i == nodeIndexBeforeLast)
			optimizedPath.push_back(path[nodeIndexBeforeLast]);

		optimizedPath.push_back(path[path.size() - 1]);

		path = optimizedPath;
	}

	return path;
}

bool WaypointManager::isLineClear(int ySecond, int yFirst, int xSecond,
								  int xFirst, int smallX, int bigX)
{
	bool isClear = true;
	float shipua = (float) ((ySecond - yFirst)) / (xSecond - xFirst);
	for (double i = smallX; (i < bigX && isClear); i += 0.6)
	{
		int y = ((float) (shipua) * i) - ((float) (shipua) * xFirst) + yFirst;
		if (map_->IsInflatedOccupied(i, y))
			isClear = false;
	}

	return isClear;
}

bool WaypointManager::IsPathClear(Node * firstNode, Node * secondNode)
{
	static const int WAYPOINT_MAX_DISTANCE = 20;

	int xFirst = firstNode->location.X();
	int xSecond = secondNode->location.X();
	int yFirst = firstNode->location.Y();
	int ySecond = secondNode->location.Y();
	int distance = sqrt(pow(xFirst - xSecond, 2) + pow(yFirst - ySecond, 2));

	if(distance > WAYPOINT_MAX_DISTANCE)
		return false;

	int bigX, smallX;

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

	return isLineClear(ySecond, yFirst, xSecond, xFirst, smallX, bigX);
}
