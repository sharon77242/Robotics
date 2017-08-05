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
#include <queue>
#include <algorithm>
#include "Node.h"

#define NEIGHBORS_COUNT 4 // should be 8 when make it more complex for now he said in lesson 6 to check 4 directions
#define DIMENSION 2
#define NEIGHBORS_ROW_DELTA 0
#define NEIGHBORS_COL_DELTA 1

using namespace HamsterAPI;
using namespace std;

typedef vector<pair<int, int> > Path;

struct NodeCostComparator {
	bool operator() (const Node *n1, const Node *n2) const {
		return n1->f > n2->f;
	}
};

class Node_priority_queue : public std::priority_queue<Node*, std::vector<Node*>, NodeCostComparator>
{
public:
	bool remove(const Node* value)
	{

		std::vector<Node*>::iterator it = std::find(this->c.begin(), this->c.end(), value);
		if (it != this->c.end()) {
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		else {
			return false;
		}
	}
};

	class PathPlanner {
	private:
		//	const int Neighbors[NEIGHBORS_COUNT][DIMENSION] = {{1,0},{1,-1},{0,1},{-1,-1}, should use when use 8 directions
				//														{-1,0},{-1,1},{0,1},{1,1}};

		const int Neighbors[NEIGHBORS_COUNT][DIMENSION] = {{1,0}, {0,1}, {-1,0}, {0,-1}};
		OccupancyGrid &grid;
		int startRow, startCol;
		int endRow, endCol;
		vector<vector<Node *> > mat;

		bool IsInMapRange(int row,int col);
		void buildGraph();
		float GetDistance(Node *p1,Node *p2);
		vector<Node *> getSuccessors(Node *node);
		Node* find(priority_queue<Node *, vector<Node *>, NodeCostComparator> queue, Node *itemToFind);
		void RemoveFromQueue(priority_queue<Node *, vector<Node *>, NodeCostComparator> queue, Node *itemToRemove);
		const Path& routeToGoal(Node* currNode, Node* startNode);

	public:
		PathPlanner(OccupancyGrid &grid, int startRow, int startCol,int endRow, int endCol);
		Path computeShortestPath();
		virtual ~PathPlanner();
	};

#endif /* PATHPLANNER_H_ */
