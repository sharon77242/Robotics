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
	setOccupyWithWall();
	convertToCoarseGrid();
	initMap(*coarseGrid);
}

void Map::initMap(OccupancyGrid &grid) {
	mat = cv::Mat(grid.getHeight(), grid.getWidth(), CV_8UC3);
	for (auto i = 0; i < grid.getHeight(); i++) {
		for (auto j = 0; j < grid.getWidth(); j++) {
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

void Map::setOccupyWithWall() {
	double gridResolution = grid.getResolution(); // 0.05
	double wall = 0.3 / gridResolution / 2; // 3
	for (auto i  = 0; i < grid.getHeight(); ++i) {
		for (auto j = 0; j < grid.getWidth(); ++j) {
			if(grid.getCell(j, i) == CELL_OCCUPIED)
			for (auto row = i - wall; row < i + wall && row < grid.getHeight() && row >= 0; ++row) {
				for (auto col = j - wall; col < j + wall && col < grid.getWidth() && col >= 0; ++col) {
					grid.setCell(j, i, CELL_OCCUPIED);
				}
			}
		}
	}
}

void Map::initCell(OccupancyGrid &grid,int i, int j) {
	Cell c = grid.getCell(j, i);
	if(i == startPos.getY() && j == startPos.getX())
	{
		paintCell(i, j, 255, 0, 0);
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
		paintCell(i, j, 0, 0, 0);
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
					if (grid.getCell(m, k) != CELL_FREE) {
						isOccupied = true;
						break;
					}
				}
			}
			if (isOccupied)
				coarseGrid->setCell(j, i, CELL_OCCUPIED);
			else
				coarseGrid->setCell(j, i, CELL_FREE);
		}
	}
}

OccupancyGrid Map::getCoarseGrid()
{
	return *coarseGrid;
}


void Map::show() const {
	cv::imshow("map", mat);
	cv::waitKey(1);
}

Map::~Map() {
	cv::destroyWindow("map");
}

