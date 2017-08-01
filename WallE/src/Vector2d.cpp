/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Vector2d.h"

Vector2d::Vector2d() {
	_x = 0;
	_y = 0;
}

Vector2d::Vector2d(int x, int y) {
	_x = x;
	_y = y;
}


int Vector2d::getX() {
	return _x;
}

void Vector2d::setX(int x) {
	_x = x;
}

int Vector2d::getY() {
	return _y;
}

void Vector2d::setY(int y) {
	_y = y;
}

float Vector2d::distanceBetweenVector(Vector2d vector2d) {
	double aSide = pow(static_cast<double>(_x - vector2d.getX()), 2);
	double bSide = pow(static_cast<double>(_y - vector2d.getY()), 2);
	return sqrt(aSide + bSide);
}

float Vector2d::manhattanDistance(Vector2d vector2d) {
	double xSide = abs(_x - vector2d.getX());
	double ySide = abs(_y - vector2d.getY());

	return (xSide + ySide);
}

float Vector2d::diagonalDistance(Vector2d vector2d) {
	double xSide = abs(_x - vector2d.getX());
	double ySide = abs(_y - vector2d.getY());

	return max(xSide, ySide);
}

float Vector2d::squaredEuclideanDistance(Vector2d vector2d) {
	double xSide = abs(_x - vector2d.getX());
	double ySide = abs(_y - vector2d.getY());

	return pow(xSide, 2) + pow(ySide, 2);
}

int Vector2d::hashCode() {
	int xPart = _x;
	int yPart = _y;

	int temp = yPart;

	while (temp >0)
	{
	    temp /= 10;
	    xPart *= 10;
	}

	return xPart + yPart;
}

string Vector2d::toString() {
	stringstream sx, sy;
	sx << _x;
	sy << _y;
	return "(" + sx.str() + ", " + sy.str() + ")";
}

bool Vector2d::operator ==(const Vector2d & vector2d) const {
	return (_x == vector2d._x) && (_y == vector2d._y);
}

bool Vector2d::operator !=(const Vector2d & vector2d) const {
	return (_x != vector2d._x) || (_y != vector2d._y);
}

bool Vector2d::operator <(const Vector2d & vector2d) const {
	return (_x < vector2d._x) && (_y < vector2d._y);
}

bool Vector2d::operator >(const Vector2d & vector2d) const {
	return (_x > vector2d._x) && (_y > vector2d._y);
}

bool Vector2d::operator <=(const Vector2d & vector2d) const {
	return (_x <= vector2d._x) && (_y <= vector2d._y);
}

bool Vector2d::operator >=(const Vector2d & vector2d) const {
	return (_x >= vector2d._x) && (_y >= vector2d._y);
}

Vector2d Vector2d::operator +(const Vector2d & vector2d) const {
	return Vector2d(_x + vector2d._x, _y + vector2d._y);
}

Vector2d Vector2d::operator -(const Vector2d & vector2d) const {
	return Vector2d(_x - vector2d._x, _y - vector2d._y);
}

Vector2d Vector2d::operator +(const int & number) const {
	return Vector2d(_x + number, _y + number);
}

Vector2d Vector2d::operator -(const int & number) const {
	return Vector2d(_x - number, _y - number);
}

Vector2d Vector2d::operator /(const int & number) const {
	return Vector2d(_x / number, _y / number);
}

Vector2d Vector2d::operator /(float & number) const {
	return Vector2d(ceil(_x / number), ceil(_y / number));
}

Vector2d Vector2d::operator *(const int & number) const {
	return Vector2d(_x * number, _y * number);
}

void Vector2d::operator =(const Vector2d & vector2d) {
	_x = vector2d._x;
	_y = vector2d._y;
}
