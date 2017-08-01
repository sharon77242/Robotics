/*
 * Map.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: user
 */

#include "Map.h"


Map::Map() {
	configurationManager = ConfigurationManager::getInstance();
	gridMap = NULL;
	height = 0;
	width = 0;
}

Map::~Map() {
	delete gridMap;
}


int Map::getCellValue(int column, int row, float resolution) {
	return gridMap->getCellValue(column / ceil(((getGridResolution() / resolution) / 2)),
							   row / ceil(((getGridResolution() / resolution) / 2)));
}

int Map::getCellValue(Vector2d point, float resolution) {
	return getCellValue(point.getX(), point.getY(), resolution);
}

int Map::getCellValue(Location location, float resolution) {
	return getCellValue(location.getX(), location.getY(), resolution);
}

void Map::setCellValue(int column, int row, int value, float resolution) {
	gridMap->setCellValue(round(column / ceil(((getGridResolution() / resolution) / 2))),
						round(row / ceil(((getGridResolution() / resolution) / 2))),
						value);
}

void Map::setCellValue(Vector2d point, int value, float resolution) {
	setCellValue(point.getX(), point.getY(), value, resolution);
}

void Map::setCellValue(Location location, int value, float resolution) {
	setCellValue(location.getX(), location.getY(), value, resolution);
}

void Map::initializeGrid(int width, int height) {
	setWidth(width);
	setHeight(height);

	gridMap = new Grid(ceil(getWidth() / (getGridMapResolutionRatio() / 2)),
					   ceil(getHeight() / (getGridMapResolutionRatio() / 2)),
					   UNKNOWN_CELL);
}


int Map::getWidth() {
	return width;
}

void Map::setWidth(int nWidth) {
	width = nWidth;
}

int Map::getHeight() {
	return height;
}

void Map::setHeight(int nHeight) {
	height = nHeight;
}

float Map::getGridResolution() {
	return gridResolution;
}

void Map::setGridResolution(float resolution) {
	gridResolution = resolution;
}

float Map::getMapResolution() {
	return mapResolution;
}

void Map::setMapResolution(float resolution) {
	mapResolution = resolution;
}

float Map::getGridMapResolutionRatio() {
	return getGridResolution() / getMapResolution();
}

// Reads and Saves the parameters received
void Map::getParams()
{
	strMapPath = configurationManager->getMapFilePath();
	gridResolution = configurationManager->getGridResolutionCM();
	mapResolution = configurationManager->getMapResolutionCM();
}

void Map::markRoute(list<Vector2d> route, float resolution) {
	markCells(route, ROUTE_CELL, resolution);
}
void Map::markCurrentLocation(Vector2d currLocation, float resolution) {
	list<Vector2d> cells = list<Vector2d>();
	cells.push_back(currLocation);

	markCells(cells, CURR_LOCATION, resolution);
}
void Map::markBlank(Vector2d lastLocation, float resolution) {
	list<Vector2d> cells = list<Vector2d>();
	cells.push_back(lastLocation);

	markCells(cells, FREE_CELL, resolution);
}
void Map::markParticles(list<Vector2d> particles, float resolution) {
	markCells(particles, PARTICLES, resolution);
}
void Map::markWayPoints(list<Vector2d> wayPoints, float resolution) {
	markCells(wayPoints, WAYPOINT_CELL, resolution);
}

void Map::markStartPoint(Vector2d startPoint, float resolution) {
	list<Vector2d> cells = list<Vector2d>();
	cells.push_back(startPoint);

	markCells(cells, START_LOCATION_CELL, resolution);
}

void Map::markGoalPoint(Vector2d goalPoint, float resolution) {
	list<Vector2d> cells = list<Vector2d>();
	cells.push_back(goalPoint);

	markCells(cells, GOAL_LOCATION_CELL, resolution);
}

void Map::markCells(list<Vector2d> points, int cellType, float resolution) {
	for (list<Vector2d>::iterator pointsIterator = points.begin(); pointsIterator != points.end(); ++pointsIterator) {
		Vector2d * point = pointsIterator.operator ->();

		setCellValue(point->getX(), point->getY(), cellType, resolution);
	}
}

void Map::MapToGrid() {

	int gridVectorRowsIndex = 0;
	int gridVectorColumnsIndex = 0;

	for (int rowsIndex = 0; rowsIndex < getHeight(); rowsIndex += (getGridMapResolutionRatio() / 2)) {
		for (int columnsIndex = 0; columnsIndex < getWidth() * Globals::BYTES_PER_PIXEL_IN_PNG; columnsIndex += (Globals::BYTES_PER_PIXEL_IN_PNG * (getGridMapResolutionRatio() / 2))) {
			bool isACertainCellOccupied = false;

			for (int unitedRowsIndex = rowsIndex; (unitedRowsIndex < (gridMap->getHeight() * (getGridMapResolutionRatio() / 2)) - 1) &&
												  (unitedRowsIndex < rowsIndex + (getGridMapResolutionRatio() / 2)) && !isACertainCellOccupied; unitedRowsIndex++) {
				for (int unitedColumnsIndex = columnsIndex; (unitedColumnsIndex < columnsIndex + ((getGridMapResolutionRatio() / 2) * Globals::BYTES_PER_PIXEL_IN_PNG)) &&
															(ceil(unitedColumnsIndex / Globals::BYTES_PER_PIXEL_IN_PNG) < (gridMap->getWidth() * (getGridMapResolutionRatio() / 2)) - 1) && !isACertainCellOccupied; unitedColumnsIndex += Globals::BYTES_PER_PIXEL_IN_PNG) {
					int cell = (unitedRowsIndex * (width * Globals::BYTES_PER_PIXEL_IN_PNG)) + unitedColumnsIndex;


					if (navImage.at(cell) != 255 || navImage.at(cell + 1) != 255 || navImage.at(cell + 2) != 255)
						isACertainCellOccupied = true;
				}
			}

			if (isACertainCellOccupied)
				gridMap->setCellValue(gridVectorColumnsIndex, gridVectorRowsIndex, OCCUPIED_CELL);
			else
				gridMap->setCellValue(gridVectorColumnsIndex, gridVectorRowsIndex, FREE_CELL);

			gridVectorColumnsIndex++;
		}

		gridVectorColumnsIndex = 0;
		gridVectorRowsIndex++;
	}
}

void Map::colorMap(string fileName) {
	vector<unsigned char> pngFile = vector<unsigned char>();
	vector<unsigned char> imagePixelsVector = vector<unsigned char>();

	for (int rowsIndex = 0; rowsIndex < gridMap->getHeight(); rowsIndex++){
		for(int columnsIndex = 0; columnsIndex < gridMap->getWidth(); columnsIndex++) {
			if (gridMap->getCellValue(columnsIndex, rowsIndex) == FREE_CELL) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::WHITE);
			} else if (gridMap->getCellValue(columnsIndex, rowsIndex) == START_LOCATION_CELL) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::RED);
			} else if (gridMap->getCellValue(columnsIndex, rowsIndex) == GOAL_LOCATION_CELL) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::BLUE);
			} else if (gridMap->getCellValue(columnsIndex, rowsIndex) == ROUTE_CELL) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::GREEN);
			} else if (gridMap->getCellValue(columnsIndex, rowsIndex) == WAYPOINT_CELL) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::YELLOW);
			}else if (gridMap->getCellValue(columnsIndex, rowsIndex) == CURR_LOCATION) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::PINK);
			}else if (gridMap->getCellValue(columnsIndex, rowsIndex) == PARTICLES) {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::GRAY);
			}else {
				pushRGBAColorToAVector(&imagePixelsVector, Colors::BLACK);
			}
		}
	}

	lodepng::encode(pngFile, imagePixelsVector, gridMap->getWidth(), gridMap->getHeight());
	lodepng::save_file(pngFile, fileName);
}

void Map::pushRGBAColorToAVector(vector<unsigned char> * vector, const int* color) {
	vector->push_back(color[0]);
	vector->push_back(color[1]);
	vector->push_back(color[2]);
	vector->push_back(color[3]);
}

list<Vector2d> Map::getCellsNeighborsByValue(Vector2d point, list<int> neighborsValues, float resolution) {
	list<Vector2d> neighbors = getCellsNeighbors(point, gridMap, 1);

	for (list<Vector2d>::iterator neighborsIterator = neighbors.begin(); neighborsIterator != neighbors.end(); neighborsIterator++) {
		int neighborValue = getCellValue(point, resolution);

		if (std::find(neighborsValues.begin(), neighborsValues.end(), neighborValue) == neighborsValues.end())
			neighbors.erase(neighborsIterator++);
	}

	return neighbors;
}

list<Vector2d> Map::getCellsNeighbors(Vector2d point, Grid * grid, int ratio) {
	list<Vector2d> neighbors = list<Vector2d>();

	for (int rowsIndex = point.getY() - ratio; rowsIndex <= point.getY() + ratio; rowsIndex++) {
		if (!(rowsIndex < 0 || rowsIndex >= grid->getHeight())) {
			for (int columnsIndex = point.getX() - ratio; columnsIndex <= point.getX() + ratio; columnsIndex++) {
				if (!(columnsIndex < 0 || columnsIndex >= grid->getWidth())) {
					Vector2d neighbor(columnsIndex, rowsIndex);

					if (neighbor != point)
						neighbors.push_back(neighbor);
				}
			}
		}
	}

	return neighbors;
}


void Map::printMap(string fileName) {
	ofstream vectorOutputFile((fileName).c_str());
	cout << "Map: " << endl;

	for (int rowsIndex = 0; rowsIndex < gridMap->getHeight(); rowsIndex++) {
		for (int columnsIndex = 0; columnsIndex < gridMap->getWidth(); columnsIndex++) {
			cout << gridMap->getCellValue(columnsIndex, rowsIndex);
			vectorOutputFile << gridMap->getCellValue(columnsIndex, rowsIndex);
		}

		cout << endl;
		vectorOutputFile << endl;
	}

	vectorOutputFile.close();
}

void Map::BlowMap(const char* filename)
{
	std::vector<unsigned char> image; //the raw pixels
		unsigned width, height;
		unsigned x, y, i, j;

		filename = "/home/user/Desktop/roboticLabMap.png";
		//decode
		unsigned error = lodepng::decode(image, width, height, filename);

		initializeGrid(width,height);

		//if there's an error, display it
		if (error)
			std::cout << "decoder error " << error << ": "
					<< lodepng_error_text(error) << std::endl;

		navImage.resize((width+2) * (height+2) * 4);
		unsigned char color; // Reset as BLACK color
		for (y = 1; y < height; y++)
			for (x = 1; x < width; x++) {
				if (image[y * width * 4 + x * 4 + 0] == 0
						|| image[y * width * 4 + x * 4 + 1] == 0
						|| image[y * width * 4 + x * 4 + 2]  == 0)
				{
					color = 0;
					// Color
					for(i = y-6; i <= y+6; i++)
					{
						for(j = x-6; j <= x+6; j++)
						{
							if((i >= 0) && (i < height) && (j < width) && (j >= 0))
							{
								navImage[(i) * width * 4 + j * 4 + 0] = color;
								navImage[(i) * width * 4 + j * 4 + 1] = color;
								navImage[(i) * width * 4 + j * 4 + 2] = color;
								navImage[(i) * width * 4 + j * 4 + 3] = 255;
							}
						}
					}
				}
				else
				{
					color = 255;
				}

				navImage[y * width * 4 + x * 4 + 0] = color;
				navImage[y * width * 4 + x * 4 + 1] = color;
				navImage[y * width * 4 + x * 4 + 2] = color;
				navImage[y * width * 4 + x * 4 + 3] = 255;
			}

		encodeOneStep("newMap.png", navImage, width, height);
}


