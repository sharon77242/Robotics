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
	const Pose& startPos;
	const Pose& endPos;
	cv::Mat mat;
	void initMap();
	void initCell(int i, int j);

public:
	Map(OccupancyGrid &grid, const Pose& startPos, const Pose& endPos);
	void show() const;
	virtual ~Map();
};

#endif /* MAP_H_ */
