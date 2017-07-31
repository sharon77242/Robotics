/*
 * Map.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map(OccupancyGrid &grid, double robotSize) : grid(grid) {
	cv::namedWindow("map", cv::WINDOW_NORMAL);
	robotSizeInPixels = robotSize / grid.getResolution();
	convertToCoarseGrid();
	//initMat();
}

void Map::initMat(OccupancyGrid &grid) {
	mat = cv::Mat(grid.getHeight(), grid.getWidth(), CV_8UC3);
	for (uint32_t i = 0; i < grid.getHeight(); i++) {
		for (uint32_t j = 0; j < grid.getWidth(); j++) {
			initCell(grid, i, j);
		}
	}
}

void Map::initCell(OccupancyGrid &grid, int i, int j) {
	Cell c = grid.getCell(i, j);
	if (c == CELL_FREE) {
		mat.at<cv::Vec3b>(i, j)[0] = 255;
		mat.at<cv::Vec3b>(i, j)[1] = 255;
		mat.at<cv::Vec3b>(i, j)[2] = 255;
	}
	else if (c == CELL_OCCUPIED) {
		mat.at<cv::Vec3b>(i, j)[0] = 0;
		mat.at<cv::Vec3b>(i, j)[1] = 0;
		mat.at<cv::Vec3b>(i, j)[2] = 0;
	}
	else { // Unknown
		mat.at<cv::Vec3b>(i, j)[0] = 128;
		mat.at<cv::Vec3b>(i, j)[1] = 128;
		mat.at<cv::Vec3b>(i, j)[2] = 128;
	}
}

void Map::convertToCoarseGrid() {
	int rows = grid.getHeight() / robotSizeInPixels;
	int cols = grid.getWidth() / robotSizeInPixels;
	double resolution = grid.getResolution() * robotSizeInPixels;

	coarseGrid = new OccupancyGrid(rows, cols, resolution);
	for (int i  = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int row = i * robotSizeInPixels;
			int col = j * robotSizeInPixels;

			bool isOccupied = false;
			for (int k = row; k < row + robotSizeInPixels && !isOccupied; k++) {
				for (int m = col; m < col + robotSizeInPixels; m++) {
					if (grid.getCell(k, m) != CELL_FREE) {
						isOccupied = true;
						break;
					}
				}
			}
			if (isOccupied)
				coarseGrid->setCell(i, j, CELL_OCCUPIED);
			else
				coarseGrid->setCell(i, j, CELL_FREE);
		}
	}
}

void Map::show() {
	initMat(*coarseGrid);
	cv::imshow("map", mat);
	cv::waitKey(1);
}

Map::~Map() {
	cv::destroyWindow("map");
	delete coarseGrid;
}

