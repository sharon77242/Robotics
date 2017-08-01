/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "WaypointsManager.h"

WaypointsManager::WaypointsManager(list<Vector2d> route, Map * map) {
	_map = map;

	initializeWaypoints(route);
}

void WaypointsManager::initializeWaypoints(list<Vector2d> route) {
	list<Vector2d>::iterator routePointsIterator = route.begin();
	Vector2d * sourcePoint = routePointsIterator.operator ->();
	std::advance(routePointsIterator, 1);
	Vector2d * destinationPoint = routePointsIterator.operator ->();

	WaypointsManager::Direction continuingDirection = getDirection(*sourcePoint, *destinationPoint);

	while (routePointsIterator != route.end()) {
		sourcePoint = routePointsIterator.operator ->();
		std::advance(routePointsIterator, 1);
		destinationPoint = routePointsIterator.operator ->();

		WaypointsManager::Direction currentDirection = getDirection(*sourcePoint, *destinationPoint);

		if (currentDirection != continuingDirection) {
			continuingDirection = currentDirection;

			_wayPoints.push_back(*sourcePoint);
		}
	}
}

list<Vector2d> WaypointsManager::getWaypoints(int waypointsType) {
	list<Vector2d> wayPoints = list<Vector2d>();

	int index = 1;

	for (list<Vector2d>::iterator wayPointsIterator = _wayPoints.begin(); wayPointsIterator != _wayPoints.end(); wayPointsIterator++) {
		Vector2d waypoint;

		if (index != 1 && index != 2 && index != 4 && index != 5) {
			switch (waypointsType) {
				case WAYPOINT_FOR_MAP:
					waypoint = (*wayPointsIterator.operator ->()) * (ceil(_map->getGridResolution() / _map->getMapResolution()) / 2);

					break;
				case WAYPOINT_FOR_GRID:
					waypoint = *wayPointsIterator.operator ->();
					break;
			}

			wayPoints.push_back(waypoint);
		}

		index++;
	}

	return wayPoints;
}

WaypointsManager::Direction WaypointsManager::getDirection(Vector2d src,
		Vector2d dest) {
	if (dest.getY() > src.getY()) {
		if (dest.getX() < src.getX())
			return UP_LEFT;
		if (dest.getX() > src.getX())
			return UP_RIGHT;
		return UP;
	}

	if (dest.getY() < src.getY()) {
		if (dest.getX() < src.getX())
			return DOWN_LEFT;
		if (dest.getX() > src.getX())
			return DOWN_RIGHT;
		return DOWN;
	}

	if (dest.getX() < src.getX())
		return LEFT;
	if (dest.getX() > src.getX())
		return RIGHT;

	return NONE;
}
