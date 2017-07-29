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
	cv::Mat mat;
	void initMap();
	void initCell(int i, int j);
	void paintCell(uint32_t row, uint32_t col, int color);

public:
	Map(OccupancyGrid &grid);
	void show() const;
	virtual ~Map();
};

#endif /* MAP_H_ */
