/*
 * DisplayManager.h
 *
 *  Created on: Aug 1, 2017
 *      Author: user
 */

#ifndef DISPLAYMANAGER_H_
#define DISPLAYMANAGER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <math.h>
#include "Declerations.h"
#include "Particle.h"

using namespace HamsterAPI;
using namespace std;

class DisplayManager
{
public:
	DisplayManager();
	virtual ~DisplayManager();

	void createMat(OccupancyGrid &grid, cv::Mat &mat);
	cv::Mat getRealSizeMat(OccupancyGrid &grid, cv::Mat& oldMat, int robotSize);
	void drawPath(cv::Mat &mat, Path &path);
	void drawWaypoints(cv::Mat &mat, Path &wayPoints);
	void drawParticles(cv::Mat &mat, vector<Particle*> &Particles);
	void drawBestParticles(cv::Mat &mat, vector<Particle*> &particles);
	void drawHamster(cv::Mat &mat, int hamsterX, int hamsterY);
};

#endif /* DISPLAYMANAGER_H_ */
