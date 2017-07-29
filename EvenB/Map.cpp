/*
 * Map.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map(OccupancyGrid &grid) : grid(grid) {
	cv::namedWindow("map");
	initMap();
}

void Map::initMap() {
	mat = cv::Mat(grid.getHeight(), grid.getWidth(), CV_8UC3);
	for (uint32_t i = 0; i < grid.getHeight(); i++)
	{
		for (uint32_t j = 0; j < grid.getWidth(); j++)
		{
			initCell(i, j);
		}
	}
}

void Map::paintCell(uint32_t row, uint32_t col, int color) {
	mat.at<cv::Vec3b>(row, col)[0] = color;
	mat.at<cv::Vec3b>(row, col)[1] = color;
	mat.at<cv::Vec3b>(row, col)[2] = color;
}

void Map::initCell(int i, int j) {
	Cell c = grid.getCell(i, j);
	if (c == CELL_FREE) {
		paintCell(i, j, 255);
	}
	else if (c == CELL_OCCUPIED) {
		double gridResolution = grid.getResolution(); // 0.05
		double wall = 0.3 / gridResolution / 2; // 3
		for (uint32_t row = i - wall; row < i + wall; ++row)
		{
				for (uint32_t col = j - wall; col < j + wall; ++col)
				{
					paintCell(row, col, 0);
				}
		}
	}
	else { // Unknown
		paintCell(i, j, 128);
	}
}

void Map::show() const {
	cv::imshow("map", mat);
	cv::waitKey(1);
}

Map::~Map() {
	cv::destroyWindow("map");
}

