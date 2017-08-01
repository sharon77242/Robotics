/*
 * Map.h
 *
 *  Created on: Jun 12, 2016
 *      Author: user
 */

#ifndef MAP_H_
#define MAP_H_

#include "ConfigurationManager.h"
#include "Grid.h"
#include "Colors.h"
#include "Vector2d.h"
#include "Location.h"
#include "Globals.h"

#include <cmath>
#include <list>
#include <algorithm>
#include <string>

class Map {
private:
	int width;
	int height;
	ConfigurationManager * configurationManager;
	Grid* gridMap;
	float gridResolution;
	float mapResolution;
	std::vector<unsigned char> navImage; //the raw pixels

	void setWidth(int width);
	void setHeight(int height);
	void setGridResolution(float resolution);
	void setMapResolution(float resolution);
	void markCells(list<Vector2d> points, int cellType, float resolution);

public:
	static const int FREE_CELL = 0;
	static const int OCCUPIED_CELL = 1;
	static const int UNKNOWN_CELL = 2;
	static const int START_LOCATION_CELL = 3;
	static const int GOAL_LOCATION_CELL = 4;
	static const int WAYPOINT_CELL = 6;
	static const int ROUTE_CELL = 5;
	static const int CURR_LOCATION = 7;
	static const int PARTICLES = 8;

	Map();
	virtual ~Map();
	Map(Map * map);

	char* strMapPath;

	int getWidth();
	int getHeight();
	float getGridResolution();
	float getMapResolution();
	float getGridMapResolutionRatio();
	int getCellValue(int column, int row, float resolution);
	int getCellValue(Vector2d point, float resolution);
	int getCellValue(Location location, float resolution);
	void setCellValue(int column, int row, int value, float resolution);
	void setCellValue(Vector2d point, int value, float resolution);
	void setCellValue(Location location, int value, float resolution);
	void  getParams();

	void printMap(string fileName);
	void MapToGrid();
	void colorMap(string pngFilePath);
	void BlowMap(const char* filename);
	void padMapObstacles(int ratio);
	void markRoute(list<Vector2d> route, float resolution);
	void markWayPoints(list<Vector2d> wayPoints, float resolution);
	void markStartPoint(Vector2d startPoint, float resolution);
	void markGoalPoint(Vector2d goalPoint, float resolution);
	void markCurrentLocation(Vector2d currLocation, float resolution);
	void markParticles(list<Vector2d> particles, float resolution);
	void markBlank(Vector2d lastLocation, float resolution);
	void initializeGrid(int width, int height);
	void pushRGBAColorToAVector(vector<unsigned char> * vector, const int* color);
	list<Vector2d> getCellsNeighbors(Vector2d point, Grid * grid, int ratio);
	list<Vector2d> getCellsNeighborsByValue(Vector2d point, list<int> neighborsValues, float resolution);

};

#endif /* MAP_H_ */
