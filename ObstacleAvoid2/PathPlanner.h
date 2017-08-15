/*
 * PathPlanner.h
 *
 *  Created on: Jun 29, 2017
 *      Author: user
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Node.h"
#include "Declerations.h"
#include <vector>
#include <queue>
using namespace HamsterAPI;
using namespace std;

struct NodeCostComparer
{
	bool operator() (const Node* n1, const Node* n2) const
	{
		return n1->f > n2->f;
	}
};

class PathPlanner
{
public:
	PathPlanner(OccupancyGrid &grid, int startRow, int startCol);
	virtual ~PathPlanner();

	Path ComputeShortestPath(int endRow, int endCol);

private:
	OccupancyGrid &grid;
	int startRow;
	int startCol;
	vector<vector<Node *> > mat;

	void BuildGraph();
	vector<Node*> GetSuccessors(Node* node);
	double getDistanceBetweenPoints(int startRow, int startCol, int endRow, int endCol);
	double getHeuristicFunction(int startRow, int startCol, int endRow, int endCol);
	bool isBetterSuccessorExistsInOpenList(priority_queue<Node*, vector<Node*>, NodeCostComparer> queue, Node* node);
	bool isExistsInCloseList(vector<Node*> vector, Node* node);
	bool isExistsInOpenList(priority_queue<Node*, vector<Node*>, NodeCostComparer> queue, Node* node);
	void TracePath(Path& path, Node* source, Node* destinationNode);
};

#endif /* PATHPLANNER_H_ */
