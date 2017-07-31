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

using namespace HamsterAPI;
using namespace std;

typedef vector<pair<int, int> > Path;

class PathPlanner {
private:
	OccupancyGrid &grid;
	int startRow, startCol;
	vector<vector<Node *> > mat;

	void buildGraph();
	//vector<Node *> PathPlanner::getSuccessors(Node *node) const;
public:
	PathPlanner(OccupancyGrid &grid, int startRow, int startCol);
	Path computeShortestPath();
	virtual ~PathPlanner();
};

#endif /* PATHPLANNER_H_ */
