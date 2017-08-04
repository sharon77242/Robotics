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
	sleep(2);

	Pose hamsterPose(hamster->getPose());
	sleep(1);
	cout << "hamster pos" << endl;
	OccupancyGrid grid = hamster->getSLAMMap();
	cout << "hamster slammap" << endl;
	Pose startPos(hamsterPose);
	Pose endPos(hamsterPose.getX() + 3,hamsterPose.getY() + 3);
	cout << "start pos end pos" << endl;
	Map map(grid, startPos, endPos, 0.25);
	cout << "init map" << endl;
	OccupancyGrid coarseGrid = map.getCoarseGrid();
	cout << "get coarse grid" << endl;
	PathPlanner pathPlan(coarseGrid, startPos.getY(), startPos.getX(), endPos.getY(), endPos.getX());
	cout << "init path planner" << endl;
	Path path (pathPlan.computeShortestPath());
	cout << "computeShortestPath" << endl;
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



