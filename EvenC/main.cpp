/*
 * Reading map and displaying it
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "Map.h"
#include "PathPlanner.h"

using namespace std;
HamsterAPI::Hamster * hamster;
using namespace HamsterAPI;

int main() {
	hamster = new Hamster(1);
	sleep(1);

	OccupancyGrid grid = hamster->getSLAMMap();
	Pose startPos();
	Pose endPos();
	Map map(grid, startPos, endPos, 0.25);
	OccupancyGrid coarseGrid = map.getCoarseGrid();
	PathPlanner pathPlan(coarseGrid, startPos.getY(), startPos.getX(), endPos.getY(), endPos.getX());
	Path path (pathPlan.computeShortestPath());
	for(auto itr = path.begin(); itr != path.end(); ++itr)
	{
		cout << "itr->first : " << itr->first << " itr->second : " << itr->second << endl;
	}

	while (hamster->isConnected()) {
		map.show();
		sleep(0.2);
	}

	delete hamster;

	return 0;
}



