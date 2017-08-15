/*
 * WaypointsManger.h
 *
 *  Created on: Jul 23, 2017
 *      Author: user
 */

#ifndef WAYPOINTSMANGER_H_
#define WAYPOINTSMANGER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Declerations.h"

class WaypointsManger
{
public:
	WaypointsManger(HamsterAPI::OccupancyGrid& coarseGrid);
	virtual ~WaypointsManger();

	Path GetWaypoints(Path* path);

private:
	HamsterAPI::OccupancyGrid& coarseGrid;

	bool raytraceWillCollide(MyCell start, MyCell end);
	int getNextWaypoint(Path* path, int startNodeIndex);
};

#endif /* WAYPOINTSMANGER_H_ */
