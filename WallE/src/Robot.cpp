/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Robot.h"

#define m_to_cm(m) (m * 100)
#define cm_to_m(cm) (0.01 * cm)

Robot::~Robot() {
	//delete _playerClient;
	//delete _position;
	//delete _laserProxy;
}


double Robot::floorRound(double x, int precisionValue) {
	return floor(precisionValue * x) / precisionValue;
}

double Robot::getXPositionInCm() {
		return floorRound(m_to_cm(_position->GetXPos()));
	}

	double Robot::getYPositionInCm() {
		return -1 * floorRound(m_to_cm(_position->GetYPos()));
	}

	double Robot::getYawPositionInRadians() {
		double originalYaw = _position->GetYaw();
		if (originalYaw < 0) {
			return floorRound(originalYaw + 2 * M_PI);
		}

		return floorRound(originalYaw);
	}


	unsigned Robot::angleToindex(double angleInDegrees) {
		double min_angle = Globals::TOTAL_DEGREES / 2;

		int index = ((double) Globals::TOTAL_SCAN_SPAN / Globals::TOTAL_DEGREES) * (angleInDegrees + min_angle);
		if ((unsigned) index >= Globals::TOTAL_SCAN_SPAN) {
			cout << "index larger than possible." << endl;
		}

		return index;
	}

	double Robot::getLaserRangeCm(unsigned index) {
		if (index > Globals::TOTAL_SCAN_SPAN) {
			return Globals::INVALID_VALUE;
		}
		return m_to_cm(_laserProxy->GetRange(index));
	}

	double Robot::getLaserRangeCm(double angle) {
		return getLaserRangeCm(angleToindex(angle));
	}


	bool Robot::isInRange(double number, double rangeStart, double rangeEnd, bool inclusive) {
		if (inclusive) {
			return number >= rangeStart && number <= rangeEnd;
		} else {
			return number > rangeStart && number < rangeEnd;
		}
	}

	bool Robot::aboutEquals(double number, double comparee, double epsilon) {
		return isInRange(number, comparee - epsilon, comparee + epsilon);
	}

Robot::Robot(char * ip, int port) {
	_playerClient = new PlayerClient(ip, port);
	_position = new Position2dProxy(_playerClient);
	_laserProxy = new LaserProxy(_playerClient);

	double initialXPos = cm_to_m(362);
	double initialYPos = -1 * cm_to_m(305);
	double initialYaw= 20;

//	do
//	{
//	_position->SetOdometry(cm_to_m(362), -1 * cm_to_m(305), 20);
//	_playerClient->Read();
//	cout << "X: " << _position->GetXPos() << " Y: " << _position->GetYPos() << " YAW: " << _position->GetYaw() << endl;
	//}
//	while(!aboutEquals(_position->GetXPos(), initialXPos, 0.01) ||
//			!aboutEquals(_position->GetYPos(), initialYPos, 0.01) ||
//			!aboutEquals(_position->GetYaw(), initialYaw, Converter::degreesToRadians(0.5)));


	_position->SetMotorEnable(true);

//	for(int i = 0; i < 15; i++)
//		Read();
}

Vector2d Robot::getPosition() {
	return Vector2d((_position->GetXPos() * (Converter::meterToCm(1) / 2.5)) + (550 / 2),
			 	 	 	  (_position->GetYPos() * (Converter::meterToCm(-1) / 2.5)) + (380 / 2));
}

Location Robot::getLocation() {
	float degreeRobotYaw = Converter::radiansToDegrees(_position->GetYaw());

	return Location((_position->GetXPos() * (Converter::meterToCm(1) / 2.5)) + (550 / 2),
							 (_position->GetYPos() * (Converter::meterToCm(-1) / 2.5)) + (380 / 2),
							 (degreeRobotYaw < 0) ? (360 + degreeRobotYaw) : degreeRobotYaw);
}

void Robot::setRobotLocation(Location location) {
	float robotYaw = (location.getYaw() > 180) ? ((-1) * (360 - location.getYaw())) : location.getYaw();

	_position->SetOdometry((location.getX() - (550 / 2)) / (Converter::meterToCm(1) / 2.5),
						   (location.getY() - (380 / 2)) / (Converter::meterToCm(-1) / 2.5),
						   Converter::degreesToRadians(robotYaw));
}

void Robot::setSpeed(float xSpeed, float angularSpeed) {
	_position->SetSpeed(xSpeed, angularSpeed);
}

void Robot::Read() {
	_playerClient->Read();
}

vector<float> Robot::getLaserScan() {
	vector<float> laserScan;

	Read();

	for (int i = 0; i < Globals::TOTAL_DEGREES; i++) {
		laserScan.push_back(getLaserDistance(Converter::degreesToIndex(i)));
	}

	Read();

	return laserScan;
}

float Robot::getLaserDistance(int index) {
	return _laserProxy->GetRange(index);
}
