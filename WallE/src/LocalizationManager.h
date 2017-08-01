/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include <list>
#include <iostream>

#include "Map.h"
#include "Particle.h"
#include "Globals.h"

using namespace std;

class LocalizationManager {
private:
	list<Particle> _particles;
	Map * _map;

	Particle * getHighestBeliefParticle();
	void HandleParticleLists(list<Particle> particlesList, list<Particle>::iterator particlesIterator, int expectedSize);

public:
	static const float BELIEF_THRESHOLD = 0.5;

	LocalizationManager(Location startLocation, Map * map);

	void updateParticles(Location destination, vector<float> laserScan);
	Location getProbableLocation();
	float getHighestBelief();

	Map * getMap();
};

#endif
