/*
 * Grid.h
 *
 *  Created on: Jun 14, 2016
 *      Author: user
 */

#ifndef GRID_H_
#define GRID_H_

#include <vector>
#include "Vector2d.h"

using namespace std;

class Grid {
private:
	vector<int> _grid;
	Vector2d _size;

public:
	Grid(int width, int height, int value);

	int getHeight();
	int getWidth();

	int getCellValue(int column, int row);
	void setCellValue(int column, int row, int value);
	void initializeGrid(int value);
	void swap(Grid * otherGrid);
};

#endif /* GRID_H_ */
