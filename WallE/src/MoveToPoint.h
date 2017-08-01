/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef MOVETOPOINT_H_
#define MOVETOPOINT_H_

#include <cmath>

#include "Wheels.h"
#include "TurnInPlace.h"
#include "MoveForward.h"
#include "Vector2d.h"
#include "Map.h"
#include "Converter.h"
#include "Globals.h"

using namespace std;

class MoveToPoint: public Wheels {
private:
	Vector2d _goalPoint;

	TurnInPlace * _turnInPlaceWheels;
	MoveForward * _goForwardWheels;

	void initializeMoveToPointWheels();
    bool isGoalLocationReached();

    Vector2d getClosestRoutePoint(Vector2d currentPoint);

public:
	MoveToPoint(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint);
	virtual ~MoveToPoint();

	bool startCondition();
	bool stopCondition();
	void behave();
};

#endif
