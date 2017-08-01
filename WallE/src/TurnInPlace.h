/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef TURNINPLACE_H_
#define TURNINPLACE_H_

#include "Wheels.h"
#include "Converter.h"
#include "Globals.h"

#include <iostream>
#include <cmath>

using namespace std;

class TurnInPlace: public Wheels {
private:
	bool chooseDirection();
	float calculateNeededYaw();

public:
	TurnInPlace(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint);

	bool startCondition();
	bool stopCondition();
	void behave();
};

#endif
