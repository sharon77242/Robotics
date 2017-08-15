/*
 * Robot.h
 *
 *  Created on: Aug 1, 2017
 *      Author: user
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <math.h>
#include "Position.h"
#include "LocalizationManager.h"
using namespace HamsterAPI;
using namespace std;

class Robot
{
public:
	Robot(Hamster &hamster, LocalizationManager &localizationManager);
	virtual ~Robot();

    Position GetRobotPosition() const;
	double GetDeltaX() const;
	double GetDeltaY() const;
	double GetDeltaYaw() const;

	void InitPose(const Position position);
	bool MoveToWaypoint(int wayPointX, int wayPointY);
	void UpdateLocation();

private:
	Hamster &hamster;
	LocalizationManager &localizationManager;
	Position currPosition;
	Position prevPosition;

	double getRealWaypointAngle(int wayPointX, int wayPointY) const;
};

#endif /* ROBOT_H_ */
