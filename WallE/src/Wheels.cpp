/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Wheels.h"

Wheels::Wheels(Robot* robot, LocalizationManager* localizationManager, Vector2d goalPoint) {
	_isStuck = false;
	_robot = robot;
    _localizationManager = localizationManager;
    _goalPoint = goalPoint;
}

Wheels::~Wheels() {
    _Wheels.clear();
}

bool Wheels::isStuck() {
	return _isStuck;
}

void Wheels::addNext(Wheels * Wheels) {
    _Wheels.push_back(Wheels);
}

list<Wheels*> Wheels::getWheelss() {
    return _Wheels;
}

void Wheels::action() {
	Location locationBeforeAction = _robot->getLocation();

	_robot->Read();

	bool found = false;

	for (list<Wheels*>::iterator WheelsIterator = _Wheels.begin(); WheelsIterator != _Wheels.end() && !found; WheelsIterator++) {
		Wheels * currWheels = *(WheelsIterator).operator ->();

		if (currWheels->startCondition() && !currWheels->stopCondition()) {
			found = true;
			currWheels->behave();
		}
	}

	if (!found) {
		_robot->setSpeed(0, 0);
	} else {
		_robot->Read();

		Location locationAfterAction = _localizationManager->getProbableLocation();
		//Location locationAfterAction = _robot->getLocation();
		Location locationDelta = locationAfterAction - locationBeforeAction;
		cout << "delta: " << locationDelta.toString() << endl;
		_localizationManager->updateParticles(locationDelta, _robot->getLaserScan());

		if (_localizationManager->getHighestBelief() >= LocalizationManager::BELIEF_THRESHOLD) {
			cout << "Highest belief: " << _localizationManager->getHighestBelief() << endl;

			_robot->setRobotLocation(_localizationManager->getProbableLocation());

		}
	}
}
