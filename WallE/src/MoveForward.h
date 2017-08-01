/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef MOVEFORWARD_H_
#define MOVEFORWARD_H_

#include "Wheels.h"
#include "Converter.h"
#include "Globals.h"
#include "Vector2d.h"

#include <iostream>

using namespace std;

class MoveForward: public Wheels {
private:
	Vector2d _goalPoint;

public:
	MoveForward(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint);
	void behave();
	bool startCondition();
	bool stopCondition();
};

#endif
