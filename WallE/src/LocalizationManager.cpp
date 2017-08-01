/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager(Location startLocation, Map * map) {
	srand(time(NULL));
	_map = map;

	_particles.push_back(Particle(startLocation, (float) 1, map));

	list<Particle> descendantParticles = getHighestBeliefParticle()->createDescendantParticles(Globals::TOTAL_PARTICLES_AMOUNT - 1);
	_particles.insert(_particles.end(), descendantParticles.begin(), descendantParticles.end());

	cout << "particles: " << _particles.size() << endl;
}

void LocalizationManager::updateParticles(Location destination, vector<float> laserScan) {
	list<Particle> particlesForDelete = list<Particle>();
	list<Particle> particlesForMultiply = list<Particle>();

	for (list<Particle>::iterator particlesIterator = _particles.begin(); particlesIterator != _particles.end(); particlesIterator++) {
		float currentParticleBelief = particlesIterator->update(destination, laserScan);

		if (currentParticleBelief < BELIEF_THRESHOLD) {
			HandleParticleLists(particlesForDelete, particlesIterator, 4);
		}
		else {
			HandleParticleLists(particlesForMultiply, particlesIterator, 2);
		}
	}

	for (list<Particle>::iterator particlesForDeleteIterator = particlesForDelete.begin(); particlesForDeleteIterator != particlesForDelete.end(); particlesForDeleteIterator++) {
		Particle * particleForRemove = particlesForDeleteIterator.operator ->();

		_particles.remove(*particleForRemove);
	}

	if (particlesForMultiply.size() == 0)
		particlesForMultiply.push_back(*getHighestBeliefParticle());

	for (list<Particle>::iterator particlesForMultiplyIterator = particlesForMultiply.begin(); particlesForMultiplyIterator != particlesForMultiply.end(); particlesForMultiplyIterator++) {
		list<Particle> decendantParticles = particlesForMultiplyIterator->createDescendantParticles((Globals::TOTAL_PARTICLES_AMOUNT - _particles.size()) / particlesForMultiply.size());
		_particles.insert(_particles.end(), decendantParticles.begin(), decendantParticles.end());
	}
}

void LocalizationManager::HandleParticleLists (list<Particle> particlesList, list<Particle>::iterator particlesIterator, int threshold) {
	if (particlesList.size() < threshold) {
		particlesList.push_back(*particlesIterator.operator ->());
	}
	else {
		for (list<Particle>::iterator particlesListIterator = particlesList.begin(); particlesListIterator != particlesList.end(); particlesListIterator++) {
				if (particlesListIterator->getBelief() < particlesIterator->getBelief()) {
					particlesList.erase(particlesListIterator++);
					particlesList.push_back(*particlesIterator.operator ->());

					break;
				}
			}
	}

}

Particle * LocalizationManager::getHighestBeliefParticle() {
	list<Particle>::iterator particlesIterator = _particles.begin();
	Particle * highestBeliefParticle = particlesIterator.operator ->();

	for (particlesIterator = _particles.begin(); particlesIterator != _particles.end(); ++particlesIterator) {
		if (particlesIterator->getBelief() > highestBeliefParticle->getBelief()) {
			highestBeliefParticle = particlesIterator.operator ->();
		}
	}

	return highestBeliefParticle;
}

float LocalizationManager::getHighestBelief() {
	return getHighestBeliefParticle()->getBelief();
}

Location LocalizationManager::getProbableLocation() {
	return getHighestBeliefParticle()->getLocation();
}

Map * LocalizationManager::getMap() {
	return _map;
}
