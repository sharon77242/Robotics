/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <cmath>
#include "Globals.h"

using namespace std;

class Converter {
public:
	static int indexToDegrees(int index);
	static int degreesToIndex(int degrees);
	static float degreesToRadians(float degrees);
	static float radiansToDegrees(float radians);
	static int meterToCm(int meter);
	static float meterToCm(float meter);
};

#endif
