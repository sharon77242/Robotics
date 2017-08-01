/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "MoveToPoint.h"

MoveToPoint::MoveToPoint(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint) : Wheels(robot, localizationManager, goalPoint) {
	_goalPoint = goalPoint;
	_turnInPlaceWheels = new TurnInPlace(_robot, _localizationManager, _goalPoint);
	_goForwardWheels = new MoveForward(_robot, _localizationManager, _goalPoint);

	addNext(_turnInPlaceWheels);
	addNext(_goForwardWheels);
}

MoveToPoint::~MoveToPoint() {
	delete _turnInPlaceWheels;
	delete _goForwardWheels;
}

Vector2d MoveToPoint::getClosestRoutePoint(Vector2d currentPoint) {
	list<int> neighborsValues = list<int>();
	neighborsValues.push_back(0);
	neighborsValues.push_back(5);

	list<Vector2d> neighbors = _localizationManager->getMap()->getCellsNeighborsByValue(currentPoint, neighborsValues, _localizationManager->getMap()->getMapResolution());
	neighbors.sort();

	for (int iteration = 0; iteration < 2; iteration++) {
		for (list<Vector2d>::iterator neighborsIterator = neighbors.begin(); neighborsIterator != neighbors.end(); neighborsIterator++) {
			Vector2d currentNeighbor = *neighborsIterator.operator ->();

			if (_localizationManager->getMap()->getCellValue(currentNeighbor, _localizationManager->getMap()->getMapResolution()) == Map::ROUTE_CELL)
				return *neighborsIterator.operator ->();
			else if (iteration >= 1)
				return getClosestRoutePoint(currentNeighbor);
		}
	}

	return Vector2d(0,0);
}

bool MoveToPoint::isGoalLocationReached() {
	return _robot->getPosition().distanceBetweenVector(_goalPoint) <= Globals::TOLERANCE;
}

bool MoveToPoint::startCondition() {
	return true;
}

bool MoveToPoint::stopCondition() {
	return isGoalLocationReached();
}

void MoveToPoint::behave() {

}
