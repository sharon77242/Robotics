#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Position.h"

class Particle {
public:
	Position * mPosition;
	Position * gPosition;
	double belief;
};

#endif /* PARTICLE_H_ */
