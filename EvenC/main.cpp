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

void printFreeCells(const OccupancyGrid& coarseGrid) {
	for (auto i = 0; i < coarseGrid.getHeight(); i++) {
		for (auto j = 0; j < coarseGrid.getWidth(); j++) {
			Cell currCell { coarseGrid.getCell(j, i) };
			if (currCell == CELL_FREE)
				cout << "free cell  row  : " << i << " col : " << j << endl;
		}
	}
}

void paintPath(const Path& path, const Pose& startPos, const Pose& endPos, Map& map)
{
	for (auto itr : path)
	{
		if (!((itr.first == startPos.getY() && itr.second == startPos.getX()) ||
			  (itr.first == endPos.getY() && itr.second == endPos.getX())))
				map.paintCell(itr.first, itr.second, 0, 0, 200);

		cout << "itr->first : " << itr.first << " itr->second : " << itr.second
				<< endl;
	}
}

int main() {
	Hamster hamster{1};
	sleep(1);

	OccupancyGrid grid = hamster.getSLAMMap();
	Pose startPos(105, 116); // col , row
	Pose endPos(101, 146); // col , row  // this Not work!!!!!!


	Map map(grid, startPos, endPos, 0.25);
	OccupancyGrid coarseGrid = map.getCoarseGrid();

	printFreeCells(coarseGrid);

	PathPlanner pathPlan(coarseGrid, startPos.getY(), startPos.getX(), endPos.getY(), endPos.getX());

	Path path (pathPlan.computeShortestPath());

	cout << " path.size() " << path.size() << endl;

	paintPath(path, startPos, endPos, map);

	while (hamster.isConnected()) {
		map.show();
		sleep(0.2);
	}

	return 0;
}



