/*
 * Map.h
 *
 *  Created on: Apr 25, 2017
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
using namespace HamsterAPI;

class Map {
private:
	OccupancyGrid& grid;
	OccupancyGrid *coarseGrid;

	cv::Mat mat;
	int robotSizeInPixels;

	void initMat(OccupancyGrid &grid);
	void initCell(OccupancyGrid &grid, int i, int j);
	void convertToCoarseGrid();

public:
	Map(OccupancyGrid &grid, double robotSize);
	void show();
	virtual ~Map();
};

#endif /* MAP_H_ */
