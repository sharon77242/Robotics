/*
 * PathPlanner.h
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include <vector>
#include "Node.h"

#define NEIGHBORS_COUNT 4 // should be 8 when make it more complex for now he said in lesson 6 to check 4 directions
#define DIMENSION 2
#define NEIGHBORS_ROW_DELTA 0
#define NEIGHBORS_COL_DELTA 1

using namespace HamsterAPI;
using namespace std;

typedef vector<pair<int, int> > Path;

class PathPlanner {
private:
//	const int Neighbors[NEIGHBORS_COUNT][DIMENSION] = {{1,0},{1,-1},{0,1},{-1,-1}, should use when use 8 directions
//														{-1,0},{-1,1},{0,1},{1,1}};

const int Neighbors[NEIGHBORS_COUNT][DIMENSION] = {{1,0}, {0,1}, {-1,0}, {0,1}};
	OccupancyGrid &grid;
	int startRow, startCol;
	int endRow, endCol;
	vector<vector<Node *> > mat;

	bool IsInMapRange(int row,int col);
	void buildGraph();
	float GetDistance(Node *p1,Node *p2);
	vector<Node *> getSuccessors(Node *node);
public:
	PathPlanner(OccupancyGrid &grid, int startRow, int startCol,int endRow, int endCol);
	Path computeShortestPath();
	virtual ~PathPlanner();
};

#endif /* PATHPLANNER_H_ */
