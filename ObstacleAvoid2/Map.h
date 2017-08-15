/*
 * Map.h
 *
 *  Created on: Jun 24, 2017
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
using namespace HamsterAPI;

class Map
{
public:
	Map(OccupancyGrid& grid, double robotSize, double mapResolution);
	virtual ~Map();

	OccupancyGrid* getCourseGrid() const;
	OccupancyGrid* getInflatedGrid() const;
	OccupancyGrid* getTransformedGrid() const;
	OccupancyGrid& getGrid() const;

	cv::Mat getRobotScanMap(const Pose& pose, const LidarScan& scan);

private:
	int robotSizeInPixels;
	OccupancyGrid &grid;
	double mapResolution;
	OccupancyGrid *courseGrid;
	OccupancyGrid *inflatedGrid;
	OccupancyGrid *transformedGrid;

	void inflate(double radius);
	void CreateCoarseGrid();
	void TransformGrid();
};

#endif /* MAP_H_ */
