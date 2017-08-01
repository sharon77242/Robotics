/*
 * Grid.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: user
 */

#include "Grid.h"

Grid::Grid(int width, int height, int value) {
	_grid = vector<int>();
	_size = Vector2d(width, height);

	if (value != -1)
	{
		initializeGrid(value);
	}
}

int Grid::getHeight() {
	return _size.getY();
}

int Grid::getWidth() {
	return _size.getX();
}

int Grid::getCellValue(int column, int row) {
	return _grid.at((row * getWidth()) + column);
}

void Grid::setCellValue(int column, int row, int value) {
	_grid[(row * getWidth()) + column] = value;
}

void Grid::initializeGrid(int value) {
	for (int rowsIndex = 0; rowsIndex < getHeight() ; rowsIndex++)
		for (int columnsIndex = 0; columnsIndex < getWidth() ; columnsIndex++)
			_grid.push_back(value);
}

void Grid::swap(Grid * otherGrid) {
	_grid.swap(otherGrid->_grid);
}
