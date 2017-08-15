/*
 * PathPlanner.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: user
 */

#include "PathPlanner.h"
#include <math.h>
#include <algorithm>
#include <set>

PathPlanner::PathPlanner(OccupancyGrid &grid, int startRow, int startCol)
	: grid(grid), startRow(startRow), startCol(startCol)
{


}

Path PathPlanner::ComputeShortestPath(int endRow, int endCol)
{
	BuildGraph();
	Path path ;
	priority_queue<Node*, vector<Node*>, NodeCostComparer> openList;
	vector<Node*> closedList;
	Node* startNode = mat[startRow][startCol];
	openList.push(startNode);
	while (!openList.empty())
	{
		Node* currNode = openList.top();
		openList.pop();
		closedList.push_back(currNode);
		vector<Node*> successors = GetSuccessors(currNode);
		for(unsigned int i = 0; i < successors.size(); i++)
		{
			if(successors[i]->row == endRow && successors[i]->col == endCol)
			{
				successors[i]->parent = currNode;
				TracePath(path, startNode, successors[i]);
				return path;
			}
			// Blocked and not in close list
			if(!isExistsInCloseList(closedList, successors[i]))
			{
				double g, h ,f;
				g = currNode->g + 1.0;
				h = getHeuristicFunction(successors[i]->row, successors[i]->col, endRow, endCol);
				f = g + h;

				if (!isExistsInOpenList(openList, successors[i]) ||
					isBetterSuccessorExistsInOpenList(openList, successors[i]))
				{
					successors[i]->parent = currNode;
					successors[i]->g = g;
					successors[i]->h = h;
					successors[i]->f = f;
					openList.push(successors[i]);
				}
			}
		}
	}
	path.clear();
	return path;
}

void PathPlanner::TracePath(Path& path, Node* source, Node* destination)
{
	int row = destination->row;
	int col = destination->col;
	Node* parent = destination->parent;
	while (row != source->row || col != source->col)
	{
		path.push_back(pair<int, int>(row, col));
		row = parent->row;
		col = parent->col;
		parent = parent->parent;
	}
	path.push_back(pair<int, int>(source->row, source->col));
	reverse(path.begin(),path.end());
}

double PathPlanner::getHeuristicFunction(int startRow, int startCol, int endRow, int endCol)
{
	return abs(endRow - startRow) + abs(endCol - startCol);
}

double PathPlanner::getDistanceBetweenPoints(int startRow, int startCol, int endRow, int endCol)
{
	return abs(sqrt(pow((endRow - startRow), 2) + pow((endCol - startCol), 2)));
}

bool PathPlanner::isBetterSuccessorExistsInOpenList(priority_queue<Node*, vector<Node*>, NodeCostComparer> queue, Node* node)
{
	for(unsigned int i = 0; i < queue.size(); i++)
	{
		Node* nextNode = queue.top();
		queue.pop();
		if (node->row == nextNode->row && node->col == nextNode->col && node->f < nextNode->f)
			return true;
	}
	return false;
}

bool PathPlanner::isExistsInOpenList(priority_queue<Node*, vector<Node*>, NodeCostComparer> queue, Node* node)
{
	for(unsigned int i = 0; i < queue.size(); i++)
	{
		Node* nextNode = queue.top();
		queue.pop();
		if (node->row == nextNode->row && node->col == nextNode->col)
			return true;
	}
	return false;
}

bool PathPlanner::isExistsInCloseList(vector<Node*> closeList, Node* node)
{
	vector<Node*>::iterator it;
	it = std::find(closeList.begin(), closeList.end(), node);
	return it != closeList.end();
}

vector<Node*> PathPlanner::GetSuccessors(Node* node)
{
	int row = node->row;
	int col = node->col;
	vector<Node*> successors;
	// Down
	if (row < grid.getHeight() - 1 && mat[row + 1][col] != NULL)
	{
		successors.push_back(mat[row + 1][col]);
	}
	//Up
	if (row > 0 && mat[row - 1][col] != NULL)
	{
		successors.push_back(mat[row - 1][col]);
	}
	//Left
	if (col > 0 && mat[row][col - 1] != NULL)
	{
		successors.push_back(mat[row][col - 1]);
	}
	//Right
	if (col < grid.getWidth() - 1 && mat[row][col + 1] != NULL)
	{
		successors.push_back(mat[row][col + 1]);
	}
	return successors;
}

void PathPlanner::BuildGraph()
{
	int rows = grid.getHeight();
	int cols = grid.getWidth();
	mat.resize(rows);
	for (int i = 0; i < rows; i++)
	{
		mat[i].resize(cols);
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Cell c = grid.getCell(i, j);
			if (c == CELL_FREE)
			{
				Node* node = new Node();
				node->row = i;
				node->col = j;
				node->f = 0;
				mat[i][j] = node;
			}
			else
			{
				mat[i][j] = NULL;
			}
		}
	}
}

PathPlanner::~PathPlanner()
{

}

