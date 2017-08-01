#include "Particle.h"

int Particle::PARTICLE_ID_SEQUENCE = 0;

Particle::Particle(float x, float y, float yaw, float belief, Map * map) {
	_map = map;
	_belief = belief;
	Location deltaLocation = getRandomDeltaLocation();
	Location newLocation(x + deltaLocation.getX(),
								  y + deltaLocation.getY(),
								  yaw + deltaLocation.getYaw());
	_location = newLocation;
	_id = PARTICLE_ID_SEQUENCE++;
}

Particle::Particle(Location location, float belief, Map * map) {
	_map = map;
	_belief = belief;
	Location deltaLocation = getRandomDeltaLocation();
	Location newLocation = location + deltaLocation;
	_location = newLocation;
	_id = PARTICLE_ID_SEQUENCE++;
}

bool Particle::operator ==(const Particle & particle) const {
	return _id == particle._id;
}

float Particle::getBelief() {
	return _belief;
}

Location Particle::getLocation() {
	return _location;
}

float Particle::update(Location destination, vector<float> laserScan) {
	_location.setX(_location.getX() + destination.getX());
	_location.setY(_location.getY() + destination.getY());
	_location.setYaw(_location.getYaw() + destination.getYaw());
	_belief = calculateBelief(destination, laserScan);

	return _belief;
}

float Particle::calculatePredictedBelief(Location destination) {
	return calculateMotionModelProbability(destination) * getBelief();
}

float Particle::calculateBelief(Location destination, vector<float> laserScan) {
	return NORMALIZATION_FACTOR * checkObservationModel(laserScan) * calculatePredictedBelief(destination);
}

float Particle::calculateMotionModelProbability(Location destination) {
	float distance = _location.vector2dValue().distanceBetweenVector(destination.vector2dValue());
	float yaw = abs(destination.getYaw());

	float propability = 0.25;

	if (distance <= MAX_DISTANCE)
		propability += 0.25;
	if (yaw <= MAX_YAW)
		propability += 0.25;
	if (yaw == 0)
		propability += 0.25;

	return propability;
}

float Particle::checkObservationModel(vector<float> laserScan) {
	float expectedObsticlesDetected = 0;

	for (unsigned int laserDegree = 0; laserDegree < laserScan.size(); laserDegree += 6) {
		float currentLaserScan = laserScan[laserDegree];

		if (isObsticleDetectedAsExpected(currentLaserScan, laserDegree))
			expectedObsticlesDetected++;
	}

	return expectedObsticlesDetected / (laserScan.size()/6);
}

list<Particle> Particle::createDescendantParticles(int amount) {
	list<Particle> descendantParticles = list<Particle>();

	for (int index = 0; index < amount; index++) {
		descendantParticles.push_back(Particle(_location, _belief * 0.9, _map));
	}

	return descendantParticles;
}

bool Particle::isObsticleDetectedAsExpected(float laserScan, int laserDegree) {
	int correctDetectionsNumber = 0;
	int incorrectDetectionsNumber = 0;

	for (int distanceFromSpottedPoint = 1; distanceFromSpottedPoint < floor(Converter::meterToCm(laserScan)); distanceFromSpottedPoint += 10) {

		float spottedPointYaw = M_PI - Converter::degreesToRadians(getLocation().getYaw() + laserDegree);
		float deltaX = cos(spottedPointYaw) * distanceFromSpottedPoint;
		float deltaY = sin(spottedPointYaw) * distanceFromSpottedPoint;

		Vector2d spottedPoint(round(_location.getX() + deltaX), round(_location.getY() + deltaY));

		if ((spottedPoint.getX() >= 0 && spottedPoint.getX() < _map->getWidth()) &&
			(spottedPoint.getY() >= 0 && spottedPoint.getY() < _map->getHeight())) {
			int spottedPointValue = _map->getCellValue(spottedPoint, _map->getMapResolution());

			if (spottedPointValue == Map::OCCUPIED_CELL) {
				incorrectDetectionsNumber++;
			} else {
				correctDetectionsNumber++;
			}
		} else {
			break;
		}
	}

	if (laserScan < Globals::LASER_MAX_DETECTION_RANGE) {
		float spottedPointYaw = M_PI - Converter::degreesToRadians(getLocation().getYaw() + laserDegree);
		float deltaX = cos(spottedPointYaw) * floor(Converter::meterToCm(laserScan));
		float deltaY = sin(spottedPointYaw) * floor(Converter::meterToCm(laserScan));

		Vector2d spottedPoint(round(_location.getX() + deltaX), round(_location.getY() + deltaY));

		if ((spottedPoint.getX() >= 0 && spottedPoint.getX() < _map->getWidth()) &&
			(spottedPoint.getY() >= 0 && spottedPoint.getY() < _map->getHeight())) {
			int spottedPointValue = _map->getCellValue(spottedPoint, _map->getMapResolution());

			if (spottedPointValue == Map::OCCUPIED_CELL) {
				correctDetectionsNumber++;
			} else {
				incorrectDetectionsNumber++;
			}
		}
	}

	return correctDetectionsNumber > incorrectDetectionsNumber;
}

Location Particle::getRandomDeltaLocation() {
	int xBoundary = floor((_map->getWidth() / 2) * MAX_PARTICLES_RELATIVE_RATIO_CREATION);
	int yBoundary = floor((_map->getHeight() / 2) * MAX_PARTICLES_RELATIVE_RATIO_CREATION);
	int yawBoundary = floor((360 / 2) * MAX_PARTICLES_RELATIVE_YAW_CREATION);

	int x = (rand() % (2 * xBoundary)) - xBoundary;
	int y = (rand() % (2 * yBoundary)) - yBoundary;
	int yaw = (rand() % (2 * yawBoundary)) - yawBoundary;

	Location randomLocation = Location(x, y, yaw);

	return randomLocation;
}
