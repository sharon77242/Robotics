/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Converter.h"

int Converter::degreesToIndex(int degrees) {
	int indexDifference = degrees * (Globals::TOTAL_SCAN_SPAN / Globals::TOTAL_DEGREES);
	return (Globals::TOTAL_SCAN_SPAN / 2) + indexDifference;
}

int Converter::indexToDegrees(int index) {
	int relativeIndex = index - (Globals::TOTAL_SCAN_SPAN / 2);
	return relativeIndex / (Globals::TOTAL_SCAN_SPAN / Globals::TOTAL_DEGREES);
}

float Converter::degreesToRadians(float degrees) {
	return (degrees * M_PI) / 180;
}

float Converter::radiansToDegrees(float radians) {
	return (radians * 180) / M_PI;
}

int Converter::meterToCm(int meter) {
	return meter * 100;
}

float Converter::meterToCm(float meter) {
	return meter * 100;
}
