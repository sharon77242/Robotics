/*
 * LocalizationManager.cpp
 *
 *  Created on: Jul 25, 2017
 *      Author: user
 */

#include "LocalizationManager.h"

LocalizationManager::LocalizationManager(Hamster& hamster, OccupancyGrid& grid)
: particles(PARTICLES_NUM), grid(grid), hamster(hamster)
{
	srand (time(NULL));
	initParticles();
}

LocalizationManager::~LocalizationManager()
{
	for (int i = 0;i < particles.size();++i)
	{
		if (particles[i])
		{
			delete (particles[i]);
			particles[i] = NULL;
		}
	}
}

void LocalizationManager::setRandomLocation(Particle *particle)
{
	Pose startPose = hamster.getPose();
	int robotX = 470;
	int robotY = 470;
	do
	{
		particle->j = rand() % grid.getWidth();
		particle->i = rand() % grid.getHeight();
		//particle->j = RandomNumber(-100, 100) + robotY;
		//particle->i = RandomNumber(-100, 100) + robotX;
	}
	while (grid.getCell(particle->i, particle->j) != CELL_FREE);

	particle->x = ((double)particle->j - (grid.getWidth() / 2.0)) * grid.getResolution();
	particle->y = -(((double)particle->i - (grid.getHeight() / 2.0)) * grid.getResolution());
	particle->yaw = rand() % 360;
}

void LocalizationManager::initParticles()
{
	for (unsigned int i = 0; i < particles.size(); ++i)
	{
		particles[i] = new Particle();
		setRandomLocation(particles[i]);
	}
}

double LocalizationManager::computeBelief(Particle *particle)
{
	LidarScan scan = hamster.getLidarScan();
	int hits = 0;
	int misses = 0;
	double obsX;
	double obsY;
	int pixelsY;
	int pixelsX;
	double angle;
	double angleRad;
	double particleYawRad;

	for (unsigned int i = 0; i < scan.getScanSize(); ++i)
	{
		angle = scan.getScanAngleIncrement() * i * DEG2RAD;
		angleRad = angle * DEG2RAD;
		particleYawRad = particle->yaw * DEG2RAD;
		if (scan.getDistance(i) < scan.getMaxRange() - 1)
		{
			obsX = (particle->x) + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			obsY = (particle->y) + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			pixelsY = grid.getWidth() / 2.0 + obsX / grid.getResolution();
			pixelsX = (grid.getHeight() / 2.0) - (obsY / grid.getResolution());

			if (grid.getCell(pixelsX, pixelsY) == CELL_OCCUPIED)
			{
				hits++;
			}
			else
			{
				misses++;
			}
		}
	}

	return (float)hits / (hits + misses);
}

void LocalizationManager::updateParticles(double deltaX, double deltaY,double deltaYaw)
{
	Particle *particle;
	for (unsigned int i = 0; i < particles.size(); ++i)
	{
		particle = particles[i];
		particle->x += deltaX;
		particle->y += deltaY;
		//particle->yaw = getNormalizedAngle(particle->yaw + deltaYaw);
		particle->yaw += deltaYaw;
		particle->i = (grid.getHeight() / 2.0) - ((double)(particle->y) / grid.getResolution());
		particle->j = (grid.getWidth() / 2.0) + ((double)(particle->x) / grid.getResolution());
		particle->belief = computeBelief(particle);
	}
	sortParticles();
	createChildren();
}

void LocalizationManager::sortParticles()
{
	std::sort(particles.begin(), particles.end(), LocalizationManager::sortByBelief);
}

void LocalizationManager::createChildren()
{
	for (int i = 0; i < PARTICLES_REMOVE; ++i)
	{
		doChildParticle(i);
	}
}

vector<Particle*> LocalizationManager::getParticles()
{
	return particles;
}

vector<Particle*> LocalizationManager::getBestParticles(int num)
{
	sortParticles();
	vector<Particle*> bestParticles(num);
	for (int i = 0; i < num; i++)
	{
		bestParticles[i] = particles[i];
	}
	return bestParticles;
}

void LocalizationManager::doChildParticle(int i)
{
	int angle = rand() % 360;
	double angleRad = angle * DEG2RAD;
	int rand_distance  = rand() % 10;
	int randFather;
	Particle *childParticle = new Particle();
	Particle *fatherParticle;

	do
	{
		randFather = rand() % (PARTICLES_NUM - PARTICLES_REMOVE);
		fatherParticle = particles[randFather];
		childParticle->j = (fatherParticle->j + rand_distance * cos(angle));
		childParticle->i = (fatherParticle->i + rand_distance * sin(angle));
	}
	while (grid.getCell(childParticle->i, childParticle->j) != CELL_FREE);

	childParticle->x = ((double)(childParticle->j) - (grid.getWidth() / 2.0)) * grid.getResolution();
	childParticle->y = -(((double)(childParticle->i) - (grid.getHeight() / 2.0)) * grid.getResolution());
	childParticle->yaw = angle;
	childParticle->belief = computeBelief(childParticle);
	particles[particles.size() - 1 - PARTICLES_REMOVE + i] = childParticle;
}

int LocalizationManager::RandomNumber(int min, int max)
{
    return ((int(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

double LocalizationManager::getNormalizedAngle(double angle)
{
	if (angle < 0)
		return angle + 360;
	if (angle > 360)
		return angle - 360;
	return angle;
}
