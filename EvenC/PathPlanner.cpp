/*
 * PathPlanner.cpp
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#include "PathPlanner.h"
#include <queue>
#include <math.h>
#include <limits>


float maxFloat = std::numeric_limits<float>::max();

PathPlanner::PathPlanner(OccupancyGrid &grid, int startRow, int startCol,int endRow, int endCol)
: grid(grid), startRow(startRow), startCol(startCol), endRow(endRow),endCol(endCol)
{
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
			Cell c = grid.getCell(j, i);
			if (c == CELL_FREE) {
				//cout << " path plan free cell i : " << i  << " j : " << j << endl;
				Node *node = new Node();
				node->row = i;
				node->col = j;
				node->g = maxFloat;
				node->f = maxFloat;
				mat[i][j] = node;
			}
			else
				mat[i][j] = NULL;
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
			successors.push_back(mat[neighborRow][neighborCol]);
	}

	return successors;
}

Node* PathPlanner::find(priority_queue<Node *, vector<Node *>, NodeCostComparator> queue, Node *itemToFind)
{
	while (!queue.empty()) {
		Node* currNode = queue.top();
		queue.pop();

		if (*currNode  == *itemToFind)
			return currNode;
	}
	return NULL;
}

const Path& PathPlanner::routeToGoal(Node* currNode, Node* startNode)
{
	Path route;

	 cout << "got to goal " << endl;

	while (currNode->row != startNode->row || currNode->col != startNode->col)
	{
		route.push_back(make_pair(currNode->row, currNode->col));
		currNode = currNode->parent;
	}

	route.push_back(make_pair(currNode->row, currNode->col));

	return route;
}

Path PathPlanner::computeShortestPath() {
	buildGraph();

	Node_priority_queue openList;
	set<Node *> closeList;
	Node *startNode = mat[startRow][startCol];
	Node *endNode = mat[endRow][endCol];

	 (startNode)->g = 0; // for now we going in 4 simple direction
	 (startNode)->h = GetDistance(startNode,endNode);
	 (startNode)->f = (startNode)->h;

	if(startNode != nullptr && endNode != nullptr)
		openList.push(startNode);
	else
		cout << "start node or end node is nullptr" << endl;

	while (!openList.empty()) {
		Node *currNode = openList.top();

		// Check if get to  goal
		 if (*currNode == *endNode)
			 return routeToGoal(currNode, startNode);

		vector<Node *> successors = getSuccessors(currNode);

		openList.pop();
		closeList.insert(currNode);

		for(vector<Node *>::iterator it = successors.begin(); it != successors.end(); ++it) {

			if (closeList.find(*it) != closeList.end())
				continue;

			 // Calc f,h,h for successor
		 	 float g = currNode->g + 1; // for now we going in 4 simple direction
		 	 float h = GetDistance(*it,endNode);
		 	 float f = g + h;

		 	 Node* OldSuccessor = find(openList,*it);

		 	 if (OldSuccessor != NULL && (OldSuccessor->g <= g  ))
		 		 continue;
		 	 else
		 	 {
				(*it)->parent = currNode;

			 	 (*it)->g = g;
			 	 (*it)->h = h;
			 	 (*it)->f = f;

				openList.remove(OldSuccessor);
			 	openList.push(*it);
		 	 }
		}
	}

	return Path{};
}

PathPlanner::~PathPlanner() {
	// TODO Auto-generated destructor stub
}

