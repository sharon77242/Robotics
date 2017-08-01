/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

#include "Vector2d.h"
#include "Map.h"
#include "LocalizationManager.h"
#include "MoveToPoint.h"

#include <list>

using namespace std;

class WaypointsManager {
private:
	Map * _map;
	list<Vector2d> _wayPoints;

	void initializeWaypoints(list<Vector2d> route);

public:
	enum Direction {UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT, NONE};
	enum WaypointType {WAYPOINT_FOR_MAP, WAYPOINT_FOR_GRID};

	WaypointsManager();
	WaypointsManager(list<Vector2d> route, Map * map);

	list<Vector2d> getWaypoints(int waypointsType);

	static Direction getDirection(Vector2d src, Vector2d dest);
};

#endif
