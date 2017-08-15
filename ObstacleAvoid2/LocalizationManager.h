/*
 * LocalizationManager.h
 *
 *  Created on: Jul 25, 2017
 *      Author: user
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Particle.h"
#include <vector>
using namespace HamsterAPI;
using namespace std;

#define PARTICLES_NUM 1000
#define PARTICLES_REMOVE 200

class LocalizationManager
{
public:
	LocalizationManager(Hamster& hamster, OccupancyGrid& grid);
	virtual ~LocalizationManager();

	vector<Particle*> getBestParticles(int num);
	void setRandomLocation(Particle *particle);
	void initParticles();
	double computeBelief(Particle *particle);
	void updateParticles(double deltaX, double deltaY,double deltaYaw);
	vector<Particle*> getParticles();
	void sortParticles();
	void createChildren();
	void doChildParticle(int i);
	static int sortByBelief(Particle* first, Particle* second)
	{
		return (first->belief > second->belief);
	}

private:
	Hamster& hamster;
	vector<Particle*> particles;
	OccupancyGrid& grid;

	int RandomNumber(int min, int max);
	double getNormalizedAngle(double angle);
};

#endif /* LOCALIZATIONMANAGER_H_ */
