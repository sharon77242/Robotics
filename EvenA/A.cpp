/*
 * HamsterAPIClientSimpleBehaviourExample.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include <math.h>
using namespace std;
using namespace HamsterAPI;
HamsterAPI::Hamster * hamster;
#define OBS_RANGE 45
#define MIN_DIST_FROM_OBS 0.8


void printPose() {
	Pose pose = hamster->getPose();
	cout << "x: " << pose.getX() << ", y: " << pose.getY() << ", heading: " << pose.getHeading() << endl;
}

float degreesToRadians(float angle)
{
	return angle * (M_PI/ 180);
}

void printObstaclePostion(Pose pose, float beta, float dDistanceRobObs)
{
	float xRob = pose.getX(), yRob = pose.getY(), alpha = pose.getHeading();
	float sumAlphaBeta = degreesToRadians(alpha) + degreesToRadians(beta);
	float xObs = xRob + dDistanceRobObs * cos(sumAlphaBeta);
	float yObs = yRob + dDistanceRobObs * sin(sumAlphaBeta);

	cout << "xObs is : " << xObs << " yObs is : " << yObs << endl;
}

float minDistanceFromObjectOnAngle(int angle)
{
	LidarScan scan = hamster->getLidarScan();
	float min = scan.getMaxRange();
	float distance = scan.getMaxRange();
	for (int i = angle - OBS_RANGE; i <= angle + OBS_RANGE; ++i)
	{
		distance = scan.getDistance(i);
		if(min > distance)
			min = distance;
	}

	return min;
}

float minDistanceFromObjectOnLeft()
{
	return minDistanceFromObjectOnAngle(270); // 225 - 315
}

float minDistanceFromObjectOnRight()
{
	return minDistanceFromObjectOnAngle(90); // 45 - 135
}

bool isObstacleInRange(int minAngle, int maxAngle, int& i, float& distance)
{
	LidarScan scan = hamster->getLidarScan();
	distance = scan.getMaxRange();
	for (i = minAngle; i <= maxAngle; ++i)
	{
		distance = scan.getDistance(i);

		if (distance < MIN_DIST_FROM_OBS)
			return true;
	}

	i = -1;
	distance = scan.getMaxRange();
	return false;
}

bool isObstacleInFront(int& i, float& distance)
{
	return isObstacleInRange(180 - OBS_RANGE, 180 + OBS_RANGE, i, distance);// 135 - 225
}

void moveForward() {
	HamsterAPI::Log::i("Client", "Moving Forward");
	sleep(0.2);
	hamster->sendSpeed(0.4, 0.0);
}

void turnLeft() {
	int i;
	float distance;
	HamsterAPI::Log::i("Client", "Turning Left");
	while(isObstacleInFront(i, distance))
	{
		sleep(0.2);
		hamster->sendSpeed(0.1, 45.0);
	}
}

void turnRight() {
	int i;
	float distance;
	HamsterAPI::Log::i("Client", "Turning Right");
	while(isObstacleInFront(i, distance))
	{
		sleep(0.2);
		hamster->sendSpeed(0.1, -45.0);
	}
}

int main() {
	hamster = new Hamster(1);
	sleep(1);

	printPose();
	float distance;
	int i;
	float maxRange = hamster->getLidarScan().getMaxRange();
	while (hamster->isConnected())
	{
		if (isObstacleInFront(i, distance))
		{
			if (i != -1 && distance != maxRange)
				printObstaclePostion(hamster->getPose(), i, distance);

			int minLeft = minDistanceFromObjectOnLeft();
			int minRight = minDistanceFromObjectOnRight();
			if(minLeft < minRight)
				turnRight();
			else
				turnLeft();
		}
		else
		{
			moveForward();
		}

		printPose();
		sleep(0.2);
	}
}
