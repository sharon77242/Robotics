#include "Position.h"

Position::Position(double x, double y, double yaw) {
	x_ = x;
	y_ = y;
	Heading_ = yaw;
}

void Position::Update(double x, double y, double yaw)
{
	x_ = x;
	y_ = y;
	Heading_ = yaw;
}

double Position::X()
{
	return x_;
}

double Position::Y()
{
	return y_;
}

double Position::Heading()
{
	return Heading_;
}
