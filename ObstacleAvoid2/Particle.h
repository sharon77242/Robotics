/*
 * Particle.h
 *
 *  Created on: Jul 25, 2017
 *      Author: user
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

struct Particle
{
	float x, y,yaw;
	double belief;
	int i, j;
};

#endif /* PARTICLE_H_ */
