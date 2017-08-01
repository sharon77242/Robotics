/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "MoveForward.h"

MoveForward::MoveForward(Robot* robot, LocalizationManager* localizationManager, Vector2d goalPoint) : Wheels(robot, localizationManager, goalPoint) {
	_goalPoint = goalPoint;
}

bool MoveForward::startCondition() {
	return true;
}

bool MoveForward::stopCondition() {
	if (_robot->getPosition().distanceBetweenVector(_goalPoint) <= Globals::TOLERANCE)
		return true;

	for (int i = Converter::degreesToIndex(-20); i < Converter::degreesToIndex(20); i++) {
		if (_robot->getLaserDistance(i) < Globals::MINIMUM_DISTANCE_FROM_WALL) {
			return true;
		}
	}

	return false;
}

void MoveForward::behave() {
	_robot->setSpeed((float) 0.5, (float) 0);
}
