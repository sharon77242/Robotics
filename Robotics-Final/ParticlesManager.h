#ifndef PARTICLESMANAGER_H_
#define PARTICLESMANAGER_H_

#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include <cmath>
#include "Models/Particle.h"
#include "Map.h"

#define NUM_OF_PARTICLES 500
#define PARTICLES_TO_DELETE 200
#define MAX_RANDOM_RADIUS 8
#define NUM_OF_BEST_PARTICLES_TO_COPY 5
#define BELIEF_THRESHOLD 0.5

using namespace std;
using namespace HamsterAPI;

class ParticlesManager {
public:
	ParticlesManager(Hamster& hamster, Map * map);
	void InitializeParticles(Position firstMapPos);
	void ResampleParticles(double deltaX, double deltaY, double deltaYaw);
	Position GetBestParticlePosition();

	vector<Particle *> GetParticles() const;

	virtual ~ParticlesManager();

private:
	vector<Particle *> _particlesVector;
	Hamster& _hamster;
	Map * _map;
	double CalculateBelief(Particle *particle, double deltaX, double deltaY, double deltaYaw);

	//belief comparator - best will be first
	struct Particlecomparator 
	{
		bool operator()(Particle * particle1 , Particle * particle2){
			return (particle1->belief > particle2->belief);
		}
	}biggerBeliefComparator;

	void EnhanceParticles();
	Position GetRandomCellFreePosition(Position* nearPosition = NULL, float yaw =0);
	void SetToRandomLocation(Particle *particle, float beliefYaw);
	void SetToRandomCloseToPoint(Particle *particle, Position firstPosition);
	double getNextBelief (Particle* particle, int misses, int matches, double deltaX, double deltaY, double deltaYaw);


};

#endif /* PARTICLESMANAGER_H_ */
