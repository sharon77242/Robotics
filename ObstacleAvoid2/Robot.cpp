/*
 * Robot.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: user
 */

#include "Robot.h"
#include "Map.h"

Robot::Robot(Hamster &hamster, LocalizationManager &localizationManager)
: hamster(hamster), localizationManager(localizationManager)
{
	currPosition.x = 0;
	currPosition.y = 0;
	currPosition.yaw = 0;
	prevPosition.x = 0;
	prevPosition.y = 0;
	prevPosition.yaw = 0;
}

Robot::~Robot()
{

}

bool Robot::MoveToWaypoint(int wayPointX, int wayPointY)
{
	float angleToWaypoint = getRealWaypointAngle(wayPointX, wayPointY);
	//cout << "angleToWaypoint: " << angleToWaypoint << endl;
	float robotYaw = currPosition.yaw;
	//cout << "robotYaw: " << robotYaw << endl;
	if (robotYaw < 0)
		robotYaw += 360;
	int angleDiff = robotYaw - angleToWaypoint;
	//cout << "angleDiff: " << angleDiff << endl;

	if (abs(angleDiff) > 8)
	{
		if (angleDiff > 0)
		{
			if (abs(angleDiff) < 180)
				hamster.sendSpeed(0.04, -45.0);
			else
				hamster.sendSpeed(0.04, 45.0);
		}
		else
		{
			if (abs(angleDiff) < 180)
				hamster.sendSpeed(0.04, 45.0);
			else
				hamster.sendSpeed(0.04, -45.0);
		}
	}
	else
	{
		hamster.sendSpeed(0.1, 0.0);
	}
	float waypointDistance = sqrt(pow(currPosition.x - wayPointX,2) + pow(currPosition.y - wayPointY,2));
	return (waypointDistance < 0.2);
}

void Robot::InitPose(const Position position)
{
	currPosition.x = position.x;
	currPosition.y = position.y;
	currPosition.yaw = position.yaw;

	Pose initialPose;
	initialPose.setX(currPosition.x);
	initialPose.setY(currPosition.y);
	initialPose.setHeading(position.yaw);
	hamster.setInitialPose(initialPose);
	sleep(1);
	UpdateLocation();
}

Position Robot::GetRobotPosition() const
{
	return currPosition;
}

double Robot::GetDeltaX() const
{
	return currPosition.x - prevPosition.x;
}

double Robot::GetDeltaY() const
{
	return currPosition.y - prevPosition.y;
}

double Robot::GetDeltaYaw() const
{
	return currPosition.yaw - prevPosition.yaw;
}

void Robot::UpdateLocation()
{
	prevPosition.x = currPosition.x;
	prevPosition.y = currPosition.y;
	//prevPosition.yaw = getNormalizedAngle(currPosition.yaw);
	prevPosition.yaw = currPosition.yaw;

	// Use particles
	//Particle* bestParticle = localizationManger.getBestParticles(1)[0];
	//currPosition.x = bestParticle->x;
	//currPosition.y = bestParticle->y;
	//currPosition.yaw = bestParticle->yaw;

	currPosition.x = hamster.getPose().getX();
	currPosition.y = hamster.getPose().getY();
	//currPosition.yaw = getNormalizedAngle(hamster.getPose().getHeading());
	currPosition.yaw = hamster.getPose().getHeading();
}

double Robot::getRealWaypointAngle(int wayPointX, int wayPointY) const
{
	//cout << "wayPointY - currPosition.y: " << wayPointY - currPosition.y << "wayPointX - currPosition.x: " << wayPointX - currPosition.x<< endl;
	float angleBetweenRad = atan2(wayPointY - currPosition.y, wayPointX - currPosition.x);
	//cout << "angleBetweenRad: " << angleBetweenRad << endl;
	float angleBetweenDeg = angleBetweenRad * 180 / M_PI;
	//cout << "angleBetweenDeg: " << angleBetweenDeg << endl;
	if (angleBetweenDeg < 0)
		return angleBetweenDeg + 360;
	return angleBetweenDeg;
}
