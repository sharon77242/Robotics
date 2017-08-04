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
using namespace HamsterAPI;

int main() {
	HamsterAPI::Hamster *hamster{new Hamster(1)};
	sleep(1);

	OccupancyGrid grid = hamster->getSLAMMap();
	Pose startPos(105, 146); // col , row
	Pose endPos(105, 152); // col , row
//	Pose endPos(101, 146); // col , row  // this Not work!!!!!!


	Map map(grid, startPos, endPos, 0.25);
	OccupancyGrid coarseGrid = map.getCoarseGrid();

//	for (auto i = 0; i < coarseGrid.getHeight(); i++)
//	{
//		for (auto j = 0; j < coarseGrid.getWidth(); j++)
//		{
//			Cell currCell{coarseGrid.getCell(j, i)};
//			if (currCell == CELL_FREE)
//				cout << "free cell  row  : " << i << " col : " << j << endl;
//		}
//	}
//
	PathPlanner pathPlan(coarseGrid, startPos.getY(), startPos.getX(), endPos.getY(), endPos.getX());
//	cout << "PathPlanner pathPlan(coarseGrid, startPos.getY(), startPos.getX(), endPos.getY(), endPos.getX());" << endl;
//
	Path path (pathPlan.computeShortestPath());
//
	cout << " path.size() " << path.size() << endl;
//
	for(auto itr : path)
	{
		cout << "itr->first : " << itr.first << " itr->second : " << itr.second << endl;
	}

	while (hamster->isConnected()) {
		map.show();
		sleep(0.2);
	}

	delete hamster;

	return 0;
}



