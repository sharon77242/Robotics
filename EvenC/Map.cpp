/*
 * Map.cpp
 *
 *  Created on: Apr 25, 2017
 *      Author: user
 */

#include "Map.h"

Map::Map(OccupancyGrid &grid, const Pose& startPos, const Pose& endPos)
: grid(grid)
, startPos(startPos)
, endPos(endPos){
	cv::namedWindow("map");
	initMap();
}

void Map::initMap() {
	mat = cv::Mat(grid.getHeight(), grid.getWidth(), CV_8UC3);
	for (int i = 0; i < grid.getHeight(); i++) {
		for (int j = 0; j < grid.getWidth(); j++) {
			initCell(i, j);
		}
	}
}

void Map::initCell(int i, int j) {
	Cell c = grid.getCell(i, j);
	if(i == startPos.getY() && j == startPos.getX())
	{
		std::cout << "printing blue" << std::endl;
		mat.at<cv::Vec3b>(i, j)[0] = 0;
		mat.at<cv::Vec3b>(i, j)[1] = 0;
		mat.at<cv::Vec3b>(i, j)[2] = 255;
	}
	else if(i == endPos.getY() && j == endPos.getX())
	{
		std::cout << "printing green" << std::endl;
		mat.at<cv::Vec3b>(i, j)[0] = 0;
		mat.at<cv::Vec3b>(i, j)[1] = 100;
		mat.at<cv::Vec3b>(i, j)[2] = 0;
	}
	else if (c == CELL_FREE) {
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

void Map::show() const {
	cv::imshow("map", mat);
	cv::waitKey(1);
}

Map::~Map() {
	cv::destroyWindow("map");
}

