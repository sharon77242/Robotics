/*
 * DisplayManager.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: user
 */

#include "DisplayManager.h"

DisplayManager::DisplayManager()
{

}

DisplayManager::~DisplayManager()
{

}

cv::Mat DisplayManager::getRealSizeMat(OccupancyGrid &grid, cv::Mat& oldMat, int robotSize)
{
	double robotSizeInPixels = robotSize / grid.getResolution();
	int rows = grid.getWidth() * robotSizeInPixels;
	int cols = grid.getHeight() * robotSizeInPixels;
	cv::Mat mat = cv::Mat(rows, cols, CV_8UC3);
	for (unsigned int i = 0; i < grid.getWidth(); i++)
	{
		for (unsigned int j = 0; j < grid.getHeight(); j++)
		{
			int row = i * robotSizeInPixels;
			int col = j * robotSizeInPixels;
			for (int k = row; k < row + robotSizeInPixels; k++)
			{
				for (int m = col; m < col + robotSizeInPixels; m++)
				{
					int r = oldMat.at<cv::Vec3b>(i, j)[0];
					int g = oldMat.at<cv::Vec3b>(i, j)[1];
					int b = oldMat.at<cv::Vec3b>(i, j)[2];
					if (k < rows - 1 && m < cols - 1)
					{
						mat.at<cv::Vec3b>(k, m)[0] = r;
						mat.at<cv::Vec3b>(k, m)[1] = g;
						mat.at<cv::Vec3b>(k, m)[2] = b;
					}
				}
			}
		}
	}
	return mat;
}

void DisplayManager::drawPath(cv::Mat &mat, Path &path)
{
	for (unsigned int i = 0; i < path.size(); i++)
	{
		int row = path[i].first;
		int col = path[i].second;
		mat.at<cv::Vec3b>(row, col)[0] = 255;
		mat.at<cv::Vec3b>(row, col)[1] = 0;
		mat.at<cv::Vec3b>(row, col)[2] = 0;
	}
}

void DisplayManager::drawWaypoints(cv::Mat &mat, Path &waypoints)
{
	for (unsigned int i = 0; i < waypoints.size(); i++)
	{
		int row = waypoints[i].first;
		int col = waypoints[i].second;
		mat.at<cv::Vec3b>(row, col)[0] = 0;
		mat.at<cv::Vec3b>(row, col)[1] = 255;
		mat.at<cv::Vec3b>(row, col)[2] = 255;
	}
}

void DisplayManager::drawParticles(cv::Mat &mat, vector<Particle*> &particles)
{
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		int row = particles[i]->j;
		int col = particles[i]->i;
		cv::circle(mat, cv::Point(row, col), 0, cv::Scalar(0, 0, 255), 0, 0, 0);
	}
}

void DisplayManager::drawBestParticles(cv::Mat &mat, vector<Particle*> &particles)
{
	for (unsigned int i = 0; i < particles.size(); i++)
	{
		int row = particles[i]->j;
		int col = particles[i]->i;
		cv::circle(mat, cv::Point(row, col), 0, cv::Scalar(0, 255, 0), 0, 0, 0);
	}
}

void DisplayManager::drawHamster(cv::Mat &mat, int hamsterX, int hamsterY)
{
	mat.at<cv::Vec3b>(hamsterX, hamsterY)[0] = 255;
	mat.at<cv::Vec3b>(hamsterX, hamsterY)[1] = 255;
	mat.at<cv::Vec3b>(hamsterX, hamsterY)[2] = 255;
}

void DisplayManager::createMat(OccupancyGrid &grid, cv::Mat &mat)
{
	for (unsigned int i = 0; i < grid.getHeight(); i++)
	{
		for (unsigned int j = 0; j < grid.getWidth(); j++)
		{
			if (grid.getCell(i, j) == CELL_FREE)
			{
				mat.at<cv::Vec3b>(i, j)[0] = 255;
				mat.at<cv::Vec3b>(i, j)[1] = 255;
				mat.at<cv::Vec3b>(i, j)[2] = 255;
			}
			else if (grid.getCell(i, j) == CELL_OCCUPIED)
			{
				mat.at<cv::Vec3b>(i, j)[0] = 0;
				mat.at<cv::Vec3b>(i, j)[1] = 0;
				mat.at<cv::Vec3b>(i, j)[2] = 0;
			}
			else
			{
				mat.at<cv::Vec3b>(i, j)[0] = 128;
				mat.at<cv::Vec3b>(i, j)[1] = 128;
				mat.at<cv::Vec3b>(i, j)[2] = 128;
			}
		}
	}
}
