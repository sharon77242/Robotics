/*
 * PathPlanner.cpp
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#include "PathPlanner.h"
#include <queue>

PathPlanner::PathPlanner(OccupancyGrid &grid, int startRow, int startCol) : grid(grid), startRow(startRow), startCol(startCol) {

}

void PathPlanner::buildGraph() {
	int rows = grid.getHeight();
	int cols = grid.getWidth();

	mat.resize(rows);
	for (int i = 0; i < rows; i++) {
		mat[i].resize(cols);
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Cell c = grid.getCell(i, j);
			if (c == CELL_FREE) {
				Node *node = new Node();
				node->row = i;
				node->col = j;
				node->f = 0;
			}
		}
	}
}

struct NodeCostComparator {
	bool operator() (const Node *n1, const Node *n2) const {
		return n1->f < n2->f;
	}
};

/*vector<Node *> PathPlanner::getSuccessors(Node *node) const {
	int row = node->row;
	int col = node->col;

	vector<Node *> successors;

	if (row < grid.getHeight() - 1 && mat[row + 1][col]) {
		successors.push_back(mat[row + 1][col]);
	}
	// TODO - Add the other neighbors
	return successors;
}*/

Path PathPlanner::computeShortestPath() {
	buildGraph();
	Path path;

	priority_queue<Node *, vector<Node *>, NodeCostComparator> openList;
	Node *startNode = mat[startRow][startCol];
	openList.push(startNode);

	while (!openList.empty()) {
		Node *currNode = openList.top();
		openList.pop();
		//vector<Node *> successors = getSuccessors(currNode);
	}


	return path;

}

PathPlanner::~PathPlanner() {
	// TODO Auto-generated destructor stub
}

