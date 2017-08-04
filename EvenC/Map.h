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

	int robotSizeInPixels;
	const Pose& startPos;
	const Pose& endPos;
	cv::Mat mat;

	// send the wanted OccupancyGrid (coarseGrid or  grid) stil not sure which one
	void initMap(OccupancyGrid &grid);
	void initCell(OccupancyGrid &grid,int i, int j);
	void paintCell(int row, int col, int firstColor, int secondColor, int thirdColor);
	void paintWithAWall(int i, int j);
	void convertToCoarseGrid();
public:
	Map(OccupancyGrid &grid, const Pose& startPos, const Pose& endPos, double robotSize);
	OccupancyGrid getCoarseGrid();
	void show() const;
	virtual ~Map();
};

#endif /* MAP_H_ */
