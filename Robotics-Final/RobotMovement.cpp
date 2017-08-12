#include "RobotMovement.h"

RobotMovement::RobotMovement(Map * map, ParticlesManager * particlesManager, Position* startPosition, MovementMode movementMode)
	: _map(map), _particalesManager(particlesManager), _movementMode(movementMode){

	_lastPosition = new Position(startPosition->X(),
								startPosition->Y(),
								startPosition->Yaw());

	_particalesManager->InitializeParticles(_map->ConevrtGlobalPositionToMapPosition(startPosition));
}

RobotMovement::~RobotMovement() {
}

void RobotMovement::GetDelts(Position * currentLocation, double &deltaXInPixel,
		double &deltaYInPixel, double &deltaYawInDegree)
{
	deltaXInPixel = currentLocation->X() - _lastPosition->X();
	deltaYInPixel = currentLocation->Y() - _lastPosition->Y();
	deltaYawInDegree = NormalizeHeading(currentLocation->Yaw() - _lastPosition->Yaw());

	this->_lastPosition->Update(currentLocation->X(), currentLocation->Y(), currentLocation->Yaw());
}

// The function returns true if the robot arrive to the waypoint, else false.
bool RobotMovement::MoveRobotToWaypoint(HamsterAPI::Hamster& hamster, Position * positionTarget)
{
	try {
		double deltaX;
		double deltaY;
		double deltaYaw;

		sleep(0.3);
		Position * currentLocation = this->GetCurrentPosition(hamster);
		float turnAngle = CalculateAngleMovement(currentLocation, positionTarget);

		if(IsArrived(currentLocation, positionTarget))
		{
			return true;
		}

		HamsterAPI::LidarScan ld = hamster.getLidarScan();

		GetDelts(currentLocation, deltaX, deltaY, deltaYaw);
		_particalesManager->ResampleParticles(deltaX,deltaY, deltaYaw);
		_map->DrawParticles(_particalesManager->GetParticles());

		if (ObstaclesInFrontOfRobot(&ld)) {
			if(turnAngle > 0)
			{
				hamster.sendSpeed(-0.5, -10);
			}
			else
			{
				hamster.sendSpeed(-0.5, 10);
			}
		}
		else if (ld.getDistance(180) < 0.6) {
			if(turnAngle > 0)
			{
				hamster.sendSpeed(0.5, 45);
			}
			else
			{
				hamster.sendSpeed(0.5, -45);
			}
		}
		else
		{
			hamster.sendSpeed(1.0, turnAngle/4);
			sleep(0.1);
		}

		// Print Location
		PrintLocationOfRobot(hamster);

	} catch (const HamsterAPI::HamsterError & message_error) {
		HamsterAPI::Log::i("Client", message_error.what());
	}

	return false;
}

void RobotMovement::PrintLocationOfRobot(const HamsterAPI::Hamster& hamster){
	Pose currentLocation = hamster.getPose();

	cout << "Robot Is At:  " << currentLocation.getX() << ", " << currentLocation.getY() << "  , Heading:  " << currentLocation.getHeading() << endl;
}

float RobotMovement::CalculateAngleMovement(Position * currentPosition, Position * targetPosition){
	float radians = atan2(targetPosition->Y() - currentPosition->Y(), targetPosition->X() - currentPosition->X());
	float angle  = radians * (180.0/PI);

	angle = NormalizeAngle(angle);

	float turnAngle = angle - currentPosition->Yaw();
	turnAngle = NormalizeAngle(turnAngle);

	return turnAngle;
}

bool RobotMovement::IsArrived(Position * currentPosition, Position * targetPosition){
	return ((currentPosition->X() > targetPosition->X() - RANGE_THRESHOLD) &&
		(currentPosition->X() < targetPosition->X() + RANGE_THRESHOLD) &&
		(currentPosition->Y() > targetPosition->Y() - RANGE_THRESHOLD) &&
		(currentPosition->Y() < targetPosition->Y() + RANGE_THRESHOLD));
}

float RobotMovement::NormalizeAngle(float angle){
	int newAngle = angle;

	if(newAngle < -180)
	{
		newAngle += 360;
	}
	else if (angle > 180)
	{
		newAngle -= 360;
	}

	return newAngle;
}

float RobotMovement::NormalizeHeading(float heading){
	float newHeading = heading;

	if(newHeading < 0)
	{
		newHeading += 360;
	}
	else if (newHeading > 360)
	{
		newHeading -= 360;
	}

	return newHeading;
}

bool RobotMovement::ObstaclesInFrontOfRobot(HamsterAPI::LidarScan * ld){
	bool obstaclesExists = false;

	for (int i = 110 ; i <= 270 ; i++)
	{
		if(ld->getDistance(i) < 0.2)
		{
			obstaclesExists = true;
			break;
		}
	}

	return obstaclesExists;
}

Position * RobotMovement::GetCurrentPosition (Hamster& hamster){
	sleep(0.1);

	if(_movementMode == MovementParticles)
	{
		return _particalesManager->GetBestParticlePosition();
	}
	else
	{
		Pose realPosition = hamster.getPose();
		Position * p = new Position(realPosition.getX(),realPosition.getY(), realPosition.getHeading());

		return p;
	}
}
