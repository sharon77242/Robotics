#include "ParticlesManager.h"
using namespace std;

ParticlesManager::ParticlesManager(Hamster *hamster, Map *map) : _hamster(hamster), _map(map)
{
}

void ParticlesManager::InitializeParticles(Position *firstMapPos)
{
	_particlesVector.resize(NUM_OF_PARTICLES);

	for (int i = 0; i < _particlesVector.size(); i++)
	{
		_particlesVector[i] = new Particle();
		SetToRandomCloseToPoint(_particlesVector[i], firstMapPos);
	}
}
Position* ParticlesManager::GetRandomCellFreePosition(Position* nearPosition, float yaw){

	int row, col, angle, radius;

	do
	{
		if(nearPosition==NULL){
			col = rand() % _map->GetWidth();
			row = rand() % _map->GetHeight();
		}
		else
		{
			angle = rand() % 360;
			radius = rand() % MAX_RANDOM_RADIUS;
			
			col = (nearPosition->X() + radius * cos(angle));
			row = (nearPosition->Y() + radius * sin(angle));
		}

	} while (_map->GetRotatedGrid()->getCell(row, col) != CELL_FREE);

	return new Position(col, row, yaw);

}
void ParticlesManager::SetToRandomLocation(Particle *particle, float beliefYaw)
{
	float randYaw = rand() % 360;

	particle->mPosition = GetRandomCellFreePosition(NULL, randYaw);
	particle->gPosition = _map->ConevrtMapPositionToGlobalPosition(particle->mPosition);
}

void ParticlesManager::SetToRandomCloseToPoint(Particle *particle, Position *firstPosition)
{
	//range of 15 degrees
	float randYaw = rand() % (30 + 1) + firstPosition->Yaw() - 15;
	if (randYaw < 0)
		randYaw += 360;
	else if (randYaw > 360)
	{
		randYaw -= 360;
	}
	particle->mPosition = GetRandomCellFreePosition(firstPosition, randYaw);//new Position(col, row, randYaw);
	particle->gPosition = _map->ConevrtMapPositionToGlobalPosition(particle->mPosition);
}
double ParticlesManager::CalculateBelief(Particle *particle, double deltaX, double deltaY, double deltaYaw)
{
	LidarScan ld = _hamster->getLidarScan();

	int matches = 0;
	int misses = 0;

	for (int i = 0; i < ld.getScanSize(); i++)
	{
		// Checks if the scan hits an object.
		if (ld.getDistance(i) < ld.getMaxRange() - 0.001)
		{
			double currentAngle = ld.getScanAngleIncrement() * i * DEG2RAD;
			double obstacleX = particle->gPosition->X() + ld.getDistance(i) * cos(currentAngle + (particle->gPosition->Yaw() - 180) * DEG2RAD);
			double obstacleY = particle->gPosition->Y() + ld.getDistance(i) * sin(currentAngle + (particle->gPosition->Yaw() - 180) * DEG2RAD);
			Position *obstaclePosition = new Position(obstacleX, obstacleY, i);

			Position *tempMapPosition = _map->ConevrtGlobalPositionToMapPosition(obstaclePosition);

			if (_map->GetRotatedGrid()->getCell(round(tempMapPosition->Y()), round(tempMapPosition->X())) == CELL_OCCUPIED)
			{
				matches++;
			}
			else if (_map->GetRotatedGrid()->getCell(round(tempMapPosition->Y()), round(tempMapPosition->X())) == CELL_FREE)
			{
				misses++;
			}

			delete (obstaclePosition);
			delete (tempMapPosition);
		}
	}

	return getNextBelief(particle, misses, matches, deltaX, deltaY, deltaYaw);
}

double ParticlesManager::getNextBelief (Particle *particle, int misses, int matches, double deltaX, double deltaY, double deltaYaw)
{
	double newBelief = ((float)matches / (matches + misses)) * (1 - fabs(deltaX) - fabs(deltaY) - (deltaYaw / 36000));
	return (particle->belief + newBelief) / 2;
}

void ParticlesManager::ResampleParticles(double deltaX, double deltaY, double deltaYaw)
{
	for (int i = 0; i < _particlesVector.size(); i++)
	{
		Particle *particle = _particlesVector[i];

		particle->gPosition->Update(particle->gPosition->X() + deltaX,
										 particle->gPosition->Y() + deltaY,
										 particle->gPosition->Yaw() + deltaYaw);

		particle->mPosition = _map->ConevrtGlobalPositionToMapPosition(particle->gPosition);

		particle->belief = CalculateBelief(particle, deltaX, deltaY, deltaYaw);

		if (_map->GetRotatedGrid()->getCell(particle->mPosition->Y(), particle->mPosition->X()) != CELL_FREE)
		{
			if (_particlesVector[i]->belief < BELIEF_THRESHOLD)
			{
				SetToRandomLocation(particle, _particlesVector[i]->gPosition->Yaw());
			}
		}
	}
	EnhanceParticles();
}

int GetWeightedRandomIndex(const vector<Particle *> &arr, int startIndex, int endIndex)
{
	int sum_of_weight = 0;
	for (int i = startIndex; i < endIndex; i++)
	{
		sum_of_weight += arr[i]->belief;
	}
	int rnd = (double)(rand() / (double)(RAND_MAX)) * sum_of_weight;
	for (int i = startIndex; i < endIndex; i++)
	{
		if (rnd < arr[i]->belief)
			return i;
		rnd -= arr[i]->belief;
	}
	return 0;
}

void ParticlesManager::EnhanceParticles()
{

	std::sort(this->_particlesVector.begin(), this->_particlesVector.end(), biggerBeliefComparator);

	for (int i = 1; i <= PARTICLES_TO_DELETE; i++)
	{
		int particleToReplaceIndex = NUM_OF_PARTICLES - i;
		int goodParticleIndex = GetWeightedRandomIndex(_particlesVector, 0, NUM_OF_BEST_PARTICLES_TO_COPY);

		float randYaw = rand() % 360;

		_particlesVector[particleToReplaceIndex]->mPosition = GetRandomCellFreePosition(_particlesVector[goodParticleIndex]->mPosition, randYaw);
		_particlesVector[particleToReplaceIndex]->gPosition = _map->ConevrtMapPositionToGlobalPosition(_particlesVector[particleToReplaceIndex]->mPosition);
		_particlesVector[particleToReplaceIndex]->belief = CalculateBelief(_particlesVector[particleToReplaceIndex], 0, 0, 0);
	}
}

vector<Particle *> ParticlesManager::GetParticles() const
{
	return _particlesVector;
}

Position *ParticlesManager::GetBestParticlePosition()
{
	std::sort(this->_particlesVector.begin(), this->_particlesVector.end(), biggerBeliefComparator);
	return this->_particlesVector[0]->gPosition;
}

ParticlesManager::~ParticlesManager()
{
}
