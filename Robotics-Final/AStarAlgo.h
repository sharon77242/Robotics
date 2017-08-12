#ifndef ASTARALGO_H_
#define ASTARALGO_H_

#include <vector>
#include <map>
#include <queue>
#include <opencv/cv.h>
#include "Models/Node.h"
#include "Models/Position.h"
#include "Map.h"

using namespace std;

struct AStarComparison
{
	bool operator()(Node *&first, Node *&second) const
	{
		return (first->GetF() > second->GetF());
	}
};

class AStarAlgo
{
  private:
	vector<Node*> _graph;
	int _graphHeight;
	int _graphWidth;
	int _gaussianRadius;

	vector<Node*> ReconstructPath(map<Node *, Node *> cameFrom, Node *current);
	double GetDistanceBetweenNodes(Node *start, Node *goal);
	vector<Node *> ConvertMapToGraph(Map* map);
	void AddNeighborIfPossible(Position currentPosition, Position neighborPosition,  Map* map, vector<Node *> *graph);
	static double GetGaussianWeight(Position currentPosition, Map* map, int gaussianRadius, double occupiedWeight);

  public:
	AStarAlgo(Map *map);
	virtual ~AStarAlgo();

	vector<Node *> AStar(Position *start, Position *goal);
};

#endif /* ASTARALGO_H_ */
