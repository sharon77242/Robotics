/*
 * PathPlanner.cpp
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#include "PathPlanner.h"
#include <queue>
#include <math.h>

PathPlanner::PathPlanner(OccupancyGrid &grid, int startRow, int startCol,int endRow, int endCol) : grid(grid), startRow(startRow), startCol(startCol), endRow(endRow),endCol(endCol) {

}

float PathPlanner::GetDistance(Node *p1,Node *p2)
{
	return sqrt(pow(abs(p1->row - p2->row), 2) + pow(abs(p1->col-p2->col), 2));
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

bool PathPlanner::IsInMapRange(int row,int col){
	return (row >= 0) && (row < grid.getHeight()) &&
			(col >= 0) && (col < grid.getWidth());
}

vector<Node *> PathPlanner::getSuccessors(Node *node){
	int row = node->row;
	int col = node->col;
	vector<Node *> successors;

	// TODO - Add the other neighbors
	for (int i = 0; i < NEIGHBORS_COUNT; ++i)
	{
		int neighborRow = row + Neighbors[i][NEIGHBORS_ROW_DELTA];
		int neighborCol = col + Neighbors[i][NEIGHBORS_COL_DELTA];

		if(IsInMapRange(neighborRow,neighborCol) && (mat[neighborRow][neighborCol]))
			successors.push_back(mat[row + 1][col]);
	}

	return successors;
}

Path PathPlanner::computeShortestPath() {
	buildGraph();

	Path route;
	priority_queue<Node *, vector<Node *>, NodeCostComparator> openList;
	Node *startNode = mat[startRow][startCol];
	Node *endNode = mat[endRow][endCol];
	openList.push(startNode);

	while (!openList.empty()) {
		Node *currNode = openList.top();
		openList.pop();
		vector<Node *> successors = getSuccessors(currNode);
		for(vector<Node *>::iterator it = successors.begin(); it != successors.end(); ++it) {
			//mat[(*it)->row][(*it)->co
		 	 (*it)->parent = currNode;
			 if ((*it)->row == endNode->row && (*it)->col == endNode->col)
			 {
				 Node* curr= (*it);
				 while(curr->row != startNode->row || curr->col != startNode->col)
				 {
					 // check if nececry to mallocreloc/resize...
					 route.push_back(make_pair(curr->row,curr->col));
				 }
				 // Todo While the going back from end to start and add all parents position (that mean is the PATH To retuen)
				 // should be here return PATH
			 }
		 	 (*it)->g = currNode->g + 1; // for now we going in 4 simple direction
		 	 (*it)->h = GetDistance(*it,endNode);
		 	 (*it)->f = (*it)->g +  (*it)->h;
//		 	 if (openList.find(*it)->f > (*it)->f ) // if we found "cheaper" way to get to this neighbor that is already neighbor of some 1 that already been checked
//		 	 {
				// cuz priority not reorder after a property changed we should swap the *it that inside with the new one with lower f
		 		// swap means remove the item and and add the new one. (not sure if priority que have the remove specifc item option
		 		// any way for making find in prio_queue best way it to inherit and create new prio_quueue with find option
//			 }
//		 	 if(its in closelist -> skip this and dont inster to open list )
//		 	 {
//		 	 }
		 	 // insert to open list new successor

		}
		// insert curNode to closeList
	}

	return route;
}

PathPlanner::~PathPlanner() {
	// TODO Auto-generated destructor stub
}

