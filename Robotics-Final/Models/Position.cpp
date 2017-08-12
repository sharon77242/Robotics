#include "Position.h"

Position::Position(double x, double y, double yaw) {
	_x = x;
	_y = y;
	_yaw = yaw;
}

void Position::Update(double x, double y, double yaw)
{
	_x = x;
	_y = y;
	_yaw = yaw;
}

double Position::X()
{
	return _x;
}

double Position::Y()
{
	return _y;
}

double Position::Yaw()
{
	return _yaw;
}

Position::~Position() {
	// TODO Auto-generated destructor stub
}

