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

Node* PathPlanner::find(priority_queue<Node *, vector<Node *>, NodeCostComparator> queue, Node *itemToFind)
{
	while (!queue.empty()) {
		Node *currNode = queue.top();
		queue.pop();

		if (*currNode  == *itemToFind)
			return currNode;
	}
	return NULL;
}

Path PathPlanner::computeShortestPath() {
	buildGraph();

	Path route;
	Node_priority_queue openList;
	set<Node *> closeList;
	Node *startNode = mat[startRow][startCol];
	Node *endNode = mat[endRow][endCol];

	if(startNode != nullptr)
		openList.push(startNode);

	while (!openList.empty()) {
		Node *currNode = openList.top();
		openList.pop();
		vector<Node *> successors = getSuccessors(currNode);

		closeList.insert(currNode);

		for(vector<Node *>::iterator it = successors.begin(); it != successors.end(); ++it) {
			//mat[(*it)->row][(*it)->co

			if (closeList.find(*it) != closeList.end())
				continue;

			(*it)->parent = currNode;

			// Check if get to  goal
			 if ((*it) == endNode)
			 {
				 Node* curr= (*it);
				 while(curr->row != startNode->row || curr->col != startNode->col)
				 {
					 route.push_back(make_pair(curr->row,curr->col));
				 }

				 return route;
			 }

			 // Calc f,h,h for successor
		 	 (*it)->g = currNode->g + 1; // for now we going in 4 simple direction
		 	 (*it)->h = GetDistance(*it,endNode);
		 	 (*it)->f = (*it)->g +  (*it)->h;

		 	 Node* OldSuccessor = find(openList,*it);
		 	 if (OldSuccessor == NULL || OldSuccessor->f <= (*it)->f)
		 		 continue;
		 	 else if (OldSuccessor != NULL && OldSuccessor->f > (*it)->f)
				openList.remove(OldSuccessor);

		 	 openList.push(*it);
		}
	}

	return route;
}

PathPlanner::~PathPlanner() {
	// TODO Auto-generated destructor stub
}

