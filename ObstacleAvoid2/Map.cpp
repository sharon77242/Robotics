/*
 * Map.cpp
 *
 *  Created on: Jun 24, 2017
 *      Author: user
 */

#include "Map.h"
#include <iostream>

Map::Map(OccupancyGrid& grid, double robotSize, double mapResolution)
: grid(grid), mapResolution(mapResolution)
{
	robotSizeInPixels = robotSize / mapResolution;
	TransformGrid();
	inflate(robotSize / 2);
	CreateCoarseGrid();
}

void Map::CreateCoarseGrid()
{
	int rows = inflatedGrid->getHeight() / robotSizeInPixels;
	int cols = inflatedGrid->getWidth() / robotSizeInPixels;
	double resolution = mapResolution * robotSizeInPixels;
	courseGrid = new OccupancyGrid(rows, cols, resolution);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int row = i * robotSizeInPixels;
			int col = j * robotSizeInPixels;
			bool isOccupied = false;
			bool isUnknown = false;
			for (int k = row; k < row + robotSizeInPixels && !isOccupied; k++)
			{
				for (int m = col; m < col + robotSizeInPixels; m++)
				{
					if (inflatedGrid->getCell(k, m) != CELL_FREE)
					{
						if (inflatedGrid->getCell(k, m) == CELL_OCCUPIED)
						{
							isOccupied = true;
						}
						else
						{
							isUnknown = true;
						}
						break;
					}
				}
			}
			if (isOccupied)
				courseGrid->setCell(i, j, CELL_OCCUPIED);
			else if (isUnknown)
				courseGrid->setCell(i, j, CELL_UNKNOWN);
			else
				courseGrid->setCell(i, j, CELL_FREE);
		}
	}
}

OccupancyGrid* Map::getTransformedGrid() const
{
	return transformedGrid;
}

OccupancyGrid* Map::getCourseGrid() const
{
	return courseGrid;
}

OccupancyGrid* Map::getInflatedGrid() const
{
	return inflatedGrid;
}

OccupancyGrid& Map::getGrid() const
{
	return grid;
}

cv::Mat Map::getRobotScanMap(const Pose& pose, const LidarScan& scan)
{
	int width = 500;
	int height = 500;
	float resolution = 0.05;
	float maxRange = scan.getMaxRange();
	cv::Mat m = cv::Mat(width, height, CV_8UC3);
	for (unsigned int i = 0; i < scan.getScanSize(); i++)
	{
		float range = scan.getDistance(i);
		if (range < maxRange)
		{
			float angle = scan.getScanAngleIncrement() * i;
			float angle_rad = angle * M_PI / 180;
			float heading = pose.getHeading();
			float heading_rad = heading * M_PI / 180;
			float robot_x = pose.getX();
			float robot_y = pose.getY();
			double meters_x = robot_x + range * cos(heading_rad + angle_rad);
			double meters_y = robot_y + range * sin(heading_rad + angle_rad);
			int pixel_x = (meters_x / resolution) + width / 2.0;
			int pixel_y = (meters_y / resolution) + height / 2.0;
			m.at<cv::Vec3b>(pixel_y, pixel_x)[0] = 255;
			m.at<cv::Vec3b>(pixel_y, pixel_x)[1] = 255;
			m.at<cv::Vec3b>(pixel_y, pixel_x)[2] = 255;
		}
	}
	return m;
}

void Map::inflate(double radius)
{
	inflatedGrid = new OccupancyGrid(*transformedGrid);
	OccupancyGrid *realGrid = new OccupancyGrid(*transformedGrid);
	int pixelsToInflate = radius / transformedGrid->getResolution();
	for (unsigned int i = 0; i < transformedGrid->getHeight(); i++)
	{
		for (unsigned int j = 0; j < transformedGrid->getWidth(); j++)
		{
			if (realGrid->getCell(i, j) == CELL_OCCUPIED)
			{
				for (unsigned int k = i - pixelsToInflate; k <= i + pixelsToInflate; k++)
				{
					for (unsigned int m = j - pixelsToInflate; m <= j + pixelsToInflate; m++)
					{
						if (m < transformedGrid->getWidth() && k < transformedGrid->getHeight())
						{
							inflatedGrid->setCell(k, m, CELL_OCCUPIED);
						}
					}
				}
			}
		}
	}
	delete realGrid;
}

void Map::TransformGrid()
{
	int width = grid.getWidth();
	int height = grid.getHeight();
	transformedGrid = new OccupancyGrid(width, height, mapResolution);

	int r, g, b;
	cv::Mat m = cv::Mat(width, height,CV_8UC3);
	Cell cell;
	int translate_col = 15;
	int translate_row = 56;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (grid.getCell(i, j) == CELL_FREE)
			{
				m.at<cv::Vec3b>(i, j)[0] = 255;
				m.at<cv::Vec3b>(i, j)[1] = 255;
				m.at<cv::Vec3b>(i, j)[2] = 255;
			}
			else if (grid.getCell(i, j) == CELL_OCCUPIED)
			{
				m.at<cv::Vec3b>(i, j)[0] = 0;
				m.at<cv::Vec3b>(i, j)[1] = 0;
				m.at<cv::Vec3b>(i, j)[2] = 0;
			}
			else
			{
				m.at<cv::Vec3b>(i, j)[0] = 128;
				m.at<cv::Vec3b>(i, j)[1] = 128;
				m.at<cv::Vec3b>(i, j)[2] = 128;
			}
		}
	}

	cv::Mat post_m;
	cv::Mat m_rotate = cv::getRotationMatrix2D(cv::Point2i(width / 2,height / 2),-30.0,1); // -30
	cv::warpAffine(m,
				   post_m,
				   m_rotate,
				   cv::Size2i(width,height),
				   cv::INTER_LINEAR,
				   cv::BORDER_CONSTANT,
				   cv::Scalar(128));

	for (int i = height - translate_row - 1; i >= 0; i--)
	{
		for (int j = width - translate_col - 1; j >= 0; j--)
		{
			r = post_m.at<cv::Vec3b>(i, j)[0];
			g = post_m.at<cv::Vec3b>(i, j)[1];
			b = post_m.at<cv::Vec3b>(i, j)[2];

			// Translate
			post_m.at<cv::Vec3b>(i+translate_row,j+translate_col)[0] = r;
			post_m.at<cv::Vec3b>(i+translate_row,j+translate_col)[1] = g;
			post_m.at<cv::Vec3b>(i+translate_row,j+translate_col)[2] = b;

			// Mark as empty
			post_m.at<cv::Vec3b>(i,j)[0] = 128;
			post_m.at<cv::Vec3b>(i,j)[1] = 128;
			post_m.at<cv::Vec3b>(i,j)[2] = 128;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (post_m.at<cv::Vec3b>(i,j)[0] == 255 &&
				post_m.at<cv::Vec3b>(i,j)[1] == 255 &&
				post_m.at<cv::Vec3b>(i,j)[2] == 255)
			{
				cell = CELL_FREE;
			}
			else if(post_m.at<cv::Vec3b>(i,j)[0] == 0 &&
					post_m.at<cv::Vec3b>(i,j)[1] == 0 &&
					post_m.at<cv::Vec3b>(i,j)[2] == 0)
			{
				cell = CELL_OCCUPIED;
			}
			else
				cell = CELL_UNKNOWN;

			transformedGrid->setCell(i,j,cell);
		}
	}
}

Map::~Map()
{
	delete courseGrid;
	delete inflatedGrid;
	delete transformedGrid;
}
