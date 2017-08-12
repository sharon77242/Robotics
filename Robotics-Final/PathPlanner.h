#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include <vector>
#include <map>
#include <queue>
#include <opencv/cv.h>
#include "Models/Node.h"
#include "Models/Position.h"
#include "Map.h"

using namespace std;

struct PathComparison
{
	bool operator()(Node *&first, Node *&second) const
	{
		return (first->calculateF() > second->calculateF());
	}
};

class PathPlanner
{
public:
	PathPlanner(Map *map);
	virtual ~PathPlanner();

	vector<Node *> AStar(Position *start, Position *goal);

  private:
	vector<Node*> _graph;
	int _graphHeight;
	int _graphWidth;
	int _gaussianRadius;

	vector<Node*> ReconstructPath(map<Node *, Node *> cameFrom, Node *current);
	double GetDistanceBetweenNodes(Node *start, Node *goal);
	vector<Node *> ConvertMapToGraph(Map* map);
	void AddNeighborIfPossible(Position currentPosition, Position neighborPosition,  Map* map, vector<Node *> *graph);
	double GetGaussianWeight(Position currentPosition, Map* map, int gaussianRadius, double occupiedWeight);
};

#endif /* PATHPLANNER_H_ */
