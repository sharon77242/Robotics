/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <list>

#include "Location.h"
#include "Map.h"
#include "Globals.h"
#include "Converter.h"

using namespace std;

class Particle {
private:
	int _id;
	Location _location;
	Map * _map;
	float _belief;

	bool isObsticleDetectedAsExpected(float laserScan, int laserDegree);
	Location getRandomDeltaLocation();

	static int PARTICLE_ID_SEQUENCE;

public:
	static const float NORMALIZATION_FACTOR = 4;
	static const float MAX_DISTANCE = 200;
	static const float MAX_YAW = 2;
	static const float MAP_MAX_CELLS_LASER = 5;
	static const float MAX_PARTICLES_RELATIVE_RATIO_CREATION = 0.015;
	static const float MAX_PARTICLES_RELATIVE_YAW_CREATION = 0.1;

	Particle(float x, float y, float yaw, float belief, Map * map);
	Particle(Location location, float belief, Map * map);

	bool operator==(const Particle & particle) const;

	float getBelief();
	Location getLocation();

	float update(Location destination, vector<float> laserScan);
	float calculatePredictedBelief(Location destination);
	float calculateBelief(Location destination, vector<float> laserScan);
	float calculateMotionModelProbability(Location destination);
	float checkObservationModel(vector<float> laserScan);
	list<Particle> createDescendantParticles(int amount);

	static float getDistance(Location destination);
};

#endif
