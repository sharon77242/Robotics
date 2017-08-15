#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include <math.h>
#include "Map.h"
#include "WaypointsManger.h"
#include "Robot.h"
#include "DisplayManager.h"
#include "LocalizationManager.h"
#include "ConfigurationManager.h"
#include "PathPlanner.h"
using namespace std;
using namespace HamsterAPI;
HamsterAPI::Hamster * hamster;

int main()
{
	try
	{
		hamster = new HamsterAPI::Hamster(1);
		sleep(3);
		cv::namedWindow("MainMap", cv::WINDOW_NORMAL);
		cv::namedWindow("ParticlesMap");

		// Get data from configuration file
		cout << "Reading file" << endl;
		ConfigurationManager::Init("ConfFile.txt");
		ConfigurationManager::PrintData();

		Position startPosition = ConfigurationManager::getStartPosition();
		double destPointX = ConfigurationManager::getDestPointX();
		double destPointY = ConfigurationManager::getDestPointY();
		double robotSize = ConfigurationManager::getRobotSize();
		double mapResolution = ConfigurationManager::getMapResolution();

		// Get hamster coarse grid
		OccupancyGrid grid = hamster->getSLAMMap();
		Map map(grid, robotSize, mapResolution);
		OccupancyGrid *transformedGrid = map.getTransformedGrid();
		OccupancyGrid *coarseGrid = map.getCourseGrid();

		// Mats creations
		cv::Mat particlesMat = cv::Mat(transformedGrid->getWidth(), transformedGrid->getHeight(), CV_8UC3);
		cv::Mat mat = cv::Mat(coarseGrid->getWidth(), coarseGrid->getHeight(), CV_8UC3);

		// Set start and dest position
		int startPosY = startPosition.x / coarseGrid->getResolution() + coarseGrid->getWidth() / 2.0;
		int startPosX = startPosition.y / coarseGrid->getResolution() + coarseGrid->getHeight() / 2.0;
		int goalPosX = (coarseGrid->getHeight() / 2.0) - (destPointY / coarseGrid->getResolution());
		int goalPosY = destPointX / coarseGrid->getResolution() + coarseGrid->getWidth() / 2.0;

		cout << startPosX << ", " << startPosY << ", " << goalPosX << ", " << goalPosY << endl;
		// Calculate hamster path
		PathPlanner pathPalnner(*coarseGrid, startPosX, startPosY);
		Path path = pathPalnner.ComputeShortestPath(goalPosX, goalPosY);
		if (path.size() == 0)
		{
			cout << "No path found" << endl;
			return 1;
		}

		// Set hamster waypoints
		WaypointsManger waypointsManger(*coarseGrid);
		Path wayPoints = waypointsManger.GetWaypoints(&path);
		if (wayPoints.size() == 0)
		{
			cout << "No waypoints found" << endl;
			return 1;
		}

		// Create localization manager
		OccupancyGrid *inflatedGrid = map.getInflatedGrid();
		LocalizationManager localizationManager(*hamster, *inflatedGrid);

		// Create Robot
		Robot robot(*hamster, localizationManager);

		// Get hamster location with particles
		vector<Particle*> particles = localizationManager.getParticles();
		vector<Particle*> topParticles = localizationManager.getBestParticles(5);

		// Display Manager
		DisplayManager displayManager;
		displayManager.createMat(*coarseGrid, mat);
		displayManager.drawHamster(mat, startPosX, startPosY);
		displayManager.drawPath(mat, path);
		displayManager.drawWaypoints(mat, wayPoints);
		//v::Mat realSizeMat(displayManager.getRealSizeMat(*coarseGrid, mat, robotSize));

		displayManager.createMat(*transformedGrid, particlesMat);
		displayManager.drawParticles(particlesMat, particles);
		displayManager.drawBestParticles(particlesMat, topParticles);

		int nextWayPoint = 0;
		float wayPointY = -(coarseGrid->getResolution() * (wayPoints[nextWayPoint].first - coarseGrid->getHeight() / 2.0));
		float wayPointX = coarseGrid->getResolution() * (wayPoints[nextWayPoint].second - coarseGrid->getWidth() / 2.0);

		cout << "CurrentPosition: (" << robot.GetRobotPosition().x << ", " << robot.GetRobotPosition().y << ")"<< endl;
		cout << "Next Waypoint: (" << wayPointX << ", " << wayPointY << ")" << endl;

		robot.InitPose(startPosition);
		hamster->setInitialPose(Pose(startPosition.x, startPosition.y, startPosition.yaw));
		//cout << "robotX: " << hamster->getPose().getX() << " robotY: " << hamster->getPose().getY() << " heading: " <<  hamster->getPose().getHeading() << endl;

		cout << "~~~~~~ Starting movement ~~~~~~" << endl;
		while (hamster->isConnected())
		{
			try
			{
				sleep(0.3);
				robot.UpdateLocation();
				localizationManager.updateParticles(robot.GetDeltaX(),robot.GetDeltaY(),robot.GetDeltaYaw());
				if (nextWayPoint < (int)wayPoints.size())
				{
					if(robot.MoveToWaypoint(wayPointX, wayPointY))
					{
						cout << "Waypoint (" << wayPointX << ", " << wayPointY << ") Reached! " << endl;
						nextWayPoint++;

						// Destination Reached
						if (nextWayPoint >= (int)wayPoints.size())
						{
							cout << "Destination (" << wayPointX << ", " << wayPointY << ") Reached! " << endl;
							hamster->sendSpeed(0.0, 0.0);
						}
						else
						{
							wayPointY = -(coarseGrid->getResolution() * (wayPoints[nextWayPoint].first - coarseGrid->getHeight() / 2.0));
							wayPointX = coarseGrid->getResolution() * (wayPoints[nextWayPoint].second - coarseGrid->getWidth() / 2.0);
							cout << "Next Waypoint: (" << wayPointX << ", " << wayPointY << ")" << endl;
							cout << "CurrentPosition: (" << robot.GetRobotPosition().x << ", " << robot.GetRobotPosition().y << ")"<< endl;
						}
					}
					localizationManager.updateParticles(robot.GetDeltaX(),robot.GetDeltaY(),robot.GetDeltaYaw());
					particles = localizationManager.getParticles();
					topParticles = localizationManager.getBestParticles(5);
					Particle* bestBeliefParticle = topParticles[0];
					cout << "Best Particle (X, Y, Yaw): " << bestBeliefParticle->x << ", " << bestBeliefParticle->y << ", " << bestBeliefParticle->yaw << endl;
					// Update particles mat
					particlesMat = cv::Mat();
					particlesMat = cv::Mat(transformedGrid->getWidth(), transformedGrid->getHeight(), CV_8UC3);
					displayManager.createMat(*transformedGrid, particlesMat);
					displayManager.drawParticles(particlesMat, particles);
					displayManager.drawBestParticles(particlesMat, topParticles);
				}

				cv::imshow("MainMap",mat);
				cv::imshow("ParticlesMap",particlesMat);
				cv::waitKey(1);
				sleep(0.5);
			}
			catch (const HamsterAPI::HamsterError & message_error)
			{
				HamsterAPI::Log::i("Client", message_error.what());
			}
		}
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}
