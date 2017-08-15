#include "PathPlanner.h"

#include <HamsterAPIClientCPP/Hamster.h>
#include <cmath>

using namespace HamsterAPI;
using namespace std;

int GAUSSIAN_RADIUS = 5;
double OCCUPIED_WEIGHT = 200;

PathPlanner::PathPlanner(Map * map)
{
	_graphWidth = map->GetWidth();
	_graphHeight = map->GetHeight();
	_graph = ConvertMapToGraph(map);
}

// A* algorithm
vector<Node *> PathPlanner::AStar(Position start, Position * goal)
{
	Node *startNode = _graph[(_graphWidth * start.Y()) + start.X()];
	Node *goalNode = _graph[(_graphWidth * goal->Y()) + goal->X()];
	map<Node *, Node *> cameFrom; // save for each Node we visit the Node we came from
	priority_queue<Node *, vector<Node *>, PathComparison> openset;

	// Use this maps to simplify the existence check of any Node
	map<Node *, bool> opensetMap;
	map<Node *, bool> closedsetMap;

	openset.push(startNode);
	opensetMap[startNode] = true;

	startNode->h = 0;
	startNode->h = GetDistanceBetweenNodes(startNode, goalNode);

	while (!openset.empty())
	{
		Node *current = openset.top();
		if (current == goalNode)
			return ReconstructPath(cameFrom, goalNode);

		// Remove the current Node from the open set
		openset.pop();
		opensetMap[current] = false;

		// Add the current Node to the closed set
		closedsetMap[current] = true;

		// Visit all neighbors
		for (unsigned int i = 0; i < current->adj.size(); i++)
		{
			Node *neighbor = current->adj[i];
			if (closedsetMap[neighbor])
				continue;
			double tentativeGScore = current->g + GetDistanceBetweenNodes(current, neighbor);
			if (!opensetMap[neighbor] || tentativeGScore < neighbor->g)
			{
				cameFrom[neighbor] = current;
				neighbor->g = tentativeGScore;
				neighbor->h = GetDistanceBetweenNodes(neighbor, goalNode);
				if (!opensetMap[neighbor])
				{
					openset.push(neighbor);
					opensetMap[neighbor] = true;
				}
			}
		}
	}
	return vector<Node *>();
}

// This is the heuristic function for the A* algorithm
double PathPlanner::GetDistanceBetweenNodes(Node *start, Node *goal)
{
	Position startLocation = start->location;
	Position goalLocation = goal->location;

	double a2 = pow(goalLocation.X() - startLocation.X(), 2);
	double b2 = pow(goalLocation.Y() - startLocation.Y(), 2);
	double dist = sqrt(a2 + b2);
	return dist;
}

vector<Node *> PathPlanner::ReconstructPath(map<Node *, Node *> cameFrom, Node *current)
{
	vector<Node *> totalPath;
	while (current)
	{
		totalPath.push_back(current);
		current = cameFrom[current];
	}
	// Now the total path is reversed(from the goal to start) so we reverse it again
	vector<Node *> reversedTotalPath;
	for (int i = totalPath.size() - 1; i >= 0; i--)
	{
		reversedTotalPath.push_back(totalPath[i]);
	}

	return reversedTotalPath;
}

vector<Node *> PathPlanner::ConvertMapToGraph(Map *map)
{
	int width = map->GetWidth(), height = map->GetHeight();
	int size = (width * height);
	vector<Node *> graph(size);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Position currentPosition (j, i);
			Node *v = new Node(currentPosition, PATH);
			int initialCost = PathPlanner::GetGaussianWeight(currentPosition, map, GAUSSIAN_RADIUS, OCCUPIED_WEIGHT);
			v->initialCost = initialCost;
			graph[(i * width) + j] = v;
		}
	}

	// Fill neighbors
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Position currentPosition(j, i);

			// Top
			Position neighborPosition(j, i - 1);
			AddNeighborIfPossible(currentPosition, neighborPosition, map, &graph);
			// Bottom
			neighborPosition = Position(j, i + 1);
			AddNeighborIfPossible(currentPosition, neighborPosition, map, &graph);
			// Right
			neighborPosition = Position(j + 1, i);
			AddNeighborIfPossible(currentPosition, neighborPosition, map, &graph);
			// Left
			neighborPosition = Position(j - 1, i);
			AddNeighborIfPossible(currentPosition, neighborPosition, map, &graph);
		}
	}

	return graph;
}

double PathPlanner::GetGaussianWeight(Position currentPosition, Map *map, int gaussianRadius, double occupiedWeight)
{
	if (map->GetGridCell(currentPosition.X(), currentPosition.Y()) != CELL_FREE)
	{
		return 0;
	}
	double val = 0;
	double wsum = 0;
	for (int iy = currentPosition.Y() - gaussianRadius;  iy <= currentPosition.Y() + gaussianRadius; iy++)
	{
		for (int jx = currentPosition.X() - gaussianRadius; jx <= currentPosition.X() + gaussianRadius; jx++)
		{

				int x = std::min((int)map->GetWidth() - 1, std::max(0, jx));
				int y = std::min((int)map->GetHeight() - 1, std::max(0, iy));

				double dsq = (jx - currentPosition.X()) * (jx - currentPosition.X()) + (iy - currentPosition.Y()) * (iy - currentPosition.Y());
				double wght=std::exp( -dsq / (2*gaussianRadius*gaussianRadius) ) / (3.14*2*gaussianRadius*gaussianRadius);

				double cellweight;
				switch(map->GetGridCell(x, y)){
				case CELL_UNKNOWN:
				case CELL_OCCUPIED:
					cellweight = OCCUPIED_WEIGHT;
					break;
				default:
					cellweight=0;
				}
				val += cellweight*wght;
				wsum += wght;
		}
	}
		return val/wsum;
}

void PathPlanner::AddNeighborIfPossible(Position currentPosition, Position neighborPosition, Map *map, vector<Node *> *graph)
{
	int width = map->GetWidth();
	int size = width * map->GetHeight();
	int currentIndex = (width * currentPosition.Y()) + currentPosition.X();
	int neighborIndex = (width * neighborPosition.Y()) + neighborPosition.X();
	if (neighborIndex > 0 &&
		neighborIndex < size &&
		map->GetGridCell(neighborPosition.X(), neighborPosition.Y()) == CELL_FREE)
	{
		(*graph)[currentIndex]->addNeighbor((*graph)[neighborIndex]);
	}
}

PathPlanner::~PathPlanner()
{
}
