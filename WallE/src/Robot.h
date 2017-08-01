/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <vector>
#include <iostream>
#include <cmath>
#include <float.h>
#include <libplayerc++/playerc++.h>

#include "LocalizationManager.h"
#include "Vector2d.h"
#include "Location.h"
#include "Converter.h"
#include "Globals.h"

using namespace PlayerCc;
using namespace std;

class Robot {
private:
	PlayerClient *_playerClient;
	Position2dProxy * _position;
	LaserProxy * _laserProxy;

public:
	virtual ~Robot();

	static bool isInRange(double number, double rangeStart, double rangeEnd, bool inclusive = true);
	static bool aboutEquals(double number, double comparee, double epsilon = DBL_EPSILON);
	static unsigned angleToindex(double angleInDegrees);
	static double floorRound(double x, int precisionValue = 10000);
	double getXPositionInCm();
	double getYPositionInCm();
	double getYawPositionInRadians();

	double getLaserRangeCm(unsigned index);
	double getLaserRangeCm(double angle);
	Robot(char* ip, int port);

	Vector2d getPosition();
	Location getLocation();
	void setRobotLocation(Location location);

	void setSpeed(float xSpeed, float angularSpeed);
	void Read();
	vector<float> getLaserScan();
	float getLaserDistance(int index);
};

#endif
