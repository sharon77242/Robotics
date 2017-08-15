/*
 * WaypointsManger.cpp
 *
 *  Created on: Jul 23, 2017
 *      Author: user
 */

#include "WaypointsManger.h"
#include <iostream>
#include <math.h>
#include "Node.h"
using namespace std;

WaypointsManger::WaypointsManger(HamsterAPI::OccupancyGrid& coarseGrid)
: coarseGrid(coarseGrid)
{

}

WaypointsManger::~WaypointsManger()
{

}

bool WaypointsManger::raytraceWillCollide(MyCell start, MyCell end)
{
    int dx = fabs(end.first - start.first);
    int dy = fabs(end.second - start.second);
    int x = start.first;
    int y = start.second;
    int n = 1 + dx + dy;
    int x_inc = (end.first > start.first) ? 1 : -1;
    int y_inc = (end.second > start.second) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

    for (; n > 0; --n)
    {
    	if(coarseGrid.getCell(x,y) != HamsterAPI::CELL_FREE)
    	{
    		return true;
    	}
    	// Continue raytrace
        if (error > 0)
        {
            x += x_inc;
            error -= dy;
        }
        else
        {
            y += y_inc;
            error += dx;
        }
    }

    return false;
}

int WaypointsManger::getNextWaypoint(Path* path, int startNodeIndex)
{
	int nextWaypoint = -1;
	MyCell targetNode;
	MyCell startNode = (*path)[startNodeIndex];
	MyCell start(startNode.first, startNode.second);

	for (unsigned int i = startNodeIndex; i < path->size() - 1; i++)
	{
		int targetNodeIndex = i + 1;
		targetNode = (*path)[targetNodeIndex];
		MyCell end(targetNode.first, targetNode.second);
		if(raytraceWillCollide(start, end))
		{
			break;
		}
		nextWaypoint = targetNodeIndex;
	}

	return nextWaypoint;
}

Path WaypointsManger::GetWaypoints(Path* path)
{
	int startNodeIndex = 0;
	Path waypoints;
	bool isDone = false;
	int pathGoalIndex = path->size() - 1;
	do
	{
		int nextWaypointIndex = getNextWaypoint(path, startNodeIndex);
		//cout << "next way point: " << nextWaypointIndex << endl;
		if (nextWaypointIndex == -1)
		{
			cout << "Did not find a next waypoint for start = " << startNodeIndex << " index" << endl;
			waypoints.clear();
			return waypoints;
		}
		else
		{
			int nextWayPointCol = (*path)[nextWaypointIndex].second;
			int nextWayPointRow = (*path)[nextWaypointIndex].first;
			//cout << "next way point Row: " << nextWayPointRow << " next way point Col: "<< nextWayPointCol <<  endl;
			waypoints.push_back(MyCell(nextWayPointRow, nextWayPointCol));
			startNodeIndex = nextWaypointIndex;

			if (pathGoalIndex == nextWaypointIndex)
				isDone = true;
		}
	}
	while(!isDone);

	return waypoints;
}

