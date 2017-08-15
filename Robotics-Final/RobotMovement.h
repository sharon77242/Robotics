#ifndef ROBOTMOVEMENT_H_
#define ROBOTMOVEMENT_H_

#include "Models/Position.h"
#include "Map.h"
#include <math.h>
#include <HamsterAPIClientCPP/Hamster.h>
#include "ParticlesManager.h"

using HamsterAPI::OccupancyGrid;
using namespace HamsterAPI;

using namespace std;

#define PI 3.14159265
#define RANGE_THRESHOLD 0.4

enum MovementMode {
	MovementPose = 0,
	MovementParticles = 1
};

class RobotMovement {
private:
	Map * _map;
	ParticlesManager * _particalesManager;
	Position * _lastPosition;
	MovementMode _movementMode;
	void GetDelts(Position currentLocation, double & deltaXInPixel, double & deltaYInPixel, double & deltaYawInDegree);
	void PrintLocationOfRobot(const HamsterAPI::Hamster& hamster);
	float CalculateAngleMovement(Position currentPosition, Position targetPosition);
	float NormalizeAngle(float angle);
	bool IsArrived(Position currentPosition, Position targetPosition);
	float NormalizeHeading(float heading);
	bool ObstaclesInFrontOfRobot(HamsterAPI::LidarScan * ld);
	Position GetCurrentPosition (Hamster& hamster);

public:
	RobotMovement(Map * map, ParticlesManager * particlesManager, Position startPosition, MovementMode movementMode);
	virtual ~RobotMovement();
	bool MoveRobotToWaypoint(HamsterAPI::Hamster& hamster, Position positionTarget);
};

#endif /* ROBOTMOVEMENT_H_ */
