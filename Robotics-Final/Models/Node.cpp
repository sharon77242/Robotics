#include "Node.h"

using namespace std;

// Approximate total cost to the target (h+g)
double Node::calculateF()
{
	return initialCost + h + g;
}

void Node::addNeighbor(Node *neighbor)
{
	adj.push_back(neighbor);
}

Node::Node(Position *location, NodeType type)
:initialCost(0)
,h(-1)
,g(-1)
,location(location)
,type(type)
{}
