#ifndef LOCATION_H_
#define LOCATION_H_

#include "Vector2d.h"
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class Location {
private:
	float _x;
	float _y;
	float _yaw;

public:
	Location();
	Location(float x, float y, float yaw = 0);
	Location(Vector2d v, float yaw = 0);

	float getX();
	void setX(float x);
	float getY();
	void setY(float y);
	float getYaw();
	void setYaw(float yaw);

	Vector2d vector2dValue();
	string toString();

	bool operator==(const Location & location) const;
	bool operator!=(const Location & location) const;
	Location operator+(const Location & location) const;
	Location operator-(const Location & location) const;
	void operator=(const Location & location);
};

#endif
