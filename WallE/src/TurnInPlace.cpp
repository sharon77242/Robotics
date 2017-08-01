/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "TurnInPlace.h"

TurnInPlace::TurnInPlace(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint): Wheels(robot, localizationManager, goalPoint) {

}

float TurnInPlace::calculateNeededYaw() {
	_robot->Read();

	Vector2d startPoint = _robot->getPosition();

	float yDeltaToPoint = abs(_goalPoint.getY() - startPoint.getY());
	float distanceToPoint = startPoint.distanceBetweenVector(_goalPoint);

	float neededYaw = acos(yDeltaToPoint / distanceToPoint);

	if (startPoint.getY() > _goalPoint.getY()) {
		if (startPoint.getX() <= _goalPoint.getX())
			return Converter::radiansToDegrees(M_PI_2 - neededYaw);
		else
			return Converter::radiansToDegrees(M_PI - neededYaw);

	}
	else {
		if (startPoint.getX() > _goalPoint.getX())
			return Converter::radiansToDegrees(M_PI + M_PI_2 - neededYaw);
		else
			return Converter::radiansToDegrees(M_PI * 2 - neededYaw);
	}

	return neededYaw;
}

bool TurnInPlace::startCondition() {
	return true;
}

bool TurnInPlace::stopCondition() {
	float neededYawDelta = calculateNeededYaw() - _robot->getLocation().getYaw();
	std::stringstream ss;
	ss << neededYawDelta;
	cout << "need to turn: " << ss.str() << endl;

	if (((neededYawDelta >= 0) && (neededYawDelta <= Globals::COMPROMISED_YAW)) ||
		((neededYawDelta < 0) && (neededYawDelta >= (-1 * Globals::COMPROMISED_YAW)))) {
		return true;
	}

	return false;
}

void TurnInPlace::behave() {
	float neededYaw = calculateNeededYaw();
	int speedFactor = 1;

	if (neededYaw - _robot->getLocation().getYaw() < 0)
		speedFactor = -1;

	_robot->setSpeed(0, (float) speedFactor * Globals::YAW_TURN_DELTA);
}
