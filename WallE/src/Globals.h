/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef GLOABLS_H_
#define GLOABLS_H_

#include <string>

using namespace std;

class Globals {
public:

	static const char CONFIGURATION_DELIMITER = ':';
	static const int TOTAL_PARTICLES_AMOUNT = 100;
	static const float TOTAL_SCAN_SPAN = 666;
	static const float TOTAL_DEGREES = 240;
	static const float LASER_MAX_DETECTION_RANGE = 4.095;
	static const float TOLERANCE = 10;
	static const float MINIMUM_DISTANCE_FROM_WALL = 0.5;
	static const int BYTES_PER_PIXEL_IN_PNG = 4;
	static const float COMPROMISED_YAW = 2;
	static const float YAW_TURN_DELTA = 0.15;
	static const float INVALID_VALUE = -1;
};

#endif
