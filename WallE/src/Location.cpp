/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Location.h"

Location::Location() {
	this->_x = 0;
	this->_y = 0;
	this->_yaw = 0;
}

Location::Location(float x, float y, float yaw) {
	this->_x = x;
	this->_y = y;
	this->_yaw = yaw;
}

Location::Location(Vector2d v, float yaw) {
	this->_x = v.getX();
	this->_y = v.getY();
	this->_yaw = yaw;
}

float Location::getX() {
	return _x;
}

void Location::setX(float x) {
	_x = x;
}

float Location::getY() {
	return _y;
}

void Location::setY(float y) {
	_y = y;
}

float Location::getYaw() {
	return _yaw;
}

void Location::setYaw(float yaw) {
	_yaw = yaw;
}

Vector2d Location::vector2dValue() {
	return Vector2d(_x, _y);
}

string Location::toString() {
	string final;
	stringstream ss;
	ss << (int) floor(getX());
	final = "(" + ss.str() + ", ";
	ss.str("");
	ss.clear();
	ss << (int) floor(getY());
	final += ss.str() + ", ";
	ss.str("");
	ss.clear();
	ss << (int) floor(getYaw());
		final += ss.str() + ")";
	return final;
}

bool Location::operator ==(const Location & location) const {
	return (_x == location._x) && (_y == location._y) && (_yaw == location._yaw);
}

bool Location::operator !=(const Location & location) const {
	return (_x != location._x) || (_y != location._y) || (_yaw != location._yaw);
}

Location Location::operator +(const Location & location) const {
	return Location(_x + location._x, _y + location._y, _yaw + location._yaw);
}

Location Location::operator -(const Location & location) const {
	return Location(_x - location._x, _y - location._y, _yaw - location._yaw);
}

void Location::operator =(const Location & location) {
	_x = location._x;
	_y = location._y;
	_yaw = location._yaw;
}
