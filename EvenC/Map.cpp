/*
 * Map.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map(OccupancyGrid &grid, const Pose& startPos, const Pose& endPos, double robotSize)
: grid(grid)
, startPos(startPos)
, endPos(endPos){
	cv::namedWindow("map", cv::WINDOW_NORMAL);
	robotSizeInPixels = robotSize / grid.getResolution();
	convertToCoarseGrid();
	initMap(*coarseGrid);
}

void Map::initMap(OccupancyGrid &grid) {
	mat = cv::Mat(grid.getHeight(), grid.getWidth(), CV_8UC3);
	for (int i = 0; i < grid.getHeight(); i++) {
		for (int j = 0; j < grid.getWidth(); j++) {
			initCell(grid,i, j);
		}
	}
}

void Map::paintCell(int row, int col, int firstColor, int secondColor, int thirdColor)
{
	if (row < 0 || col < 0 || row > grid.getHeight() || col > grid.getWidth())
		return;

	mat.at<cv::Vec3b>(row, col)[0] = firstColor;
	mat.at<cv::Vec3b>(row, col)[1] = secondColor;
	mat.at<cv::Vec3b>(row, col)[2] = thirdColor;
}

void Map::paintWithAWall(int i, int j) {
	double gridResolution = grid.getResolution(); // 0.05
	double wall = 0.3 / gridResolution / 2; // 3
	for (int row = i - wall; row < i + wall; ++row) {
		for (int col = j - wall; col < j + wall; ++col) {
			paintCell(i, j, 0, 0, 0);
		}
	}
}

void Map::initCell(OccupancyGrid &grid,int i, int j) {
	Cell c = grid.getCell(i, j);
	if(i == startPos.getY() && j == startPos.getX())
	{
		paintCell(i, j, 0, 0, 255);
		std::cout << "painting blue" << std::endl;
	}
	else if(i == endPos.getY() && j == endPos.getX())
	{
		paintCell(i, j, 0, 100, 0);
		std::cout << "printing green" << std::endl;
	}
	else if (c == CELL_FREE) {
		paintCell(i, j, 255, 255, 255);
	}
	else if (c == CELL_OCCUPIED) {
		paintWithAWall(i, j);
	}
	else { // Unknown
		paintCell(i, j, 128, 128, 128);
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

void Map::show() const {
	cv::imshow("map", mat);
	cv::waitKey(1);
}

Map::~Map() {
	cv::destroyWindow("map");
}

