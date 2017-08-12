#include "Node.h"

void Node::SetH(double h)
{
	_h = h;
}

void Node::SetG(double g)
{
	_g = g;
}

void Node::SetInitialCost(double cost){
	_initialCost=cost;
}
double Node::GetInitialCost(){
	return _initialCost;
}

double Node::GetH()
{
	return _h;
}

double Node::GetG()
{
	return _g;
}

// Approximate total cost to the target (h+g)
double Node::GetF()
{
	return _initialCost + _h + _g;
}

Position* Node::GetLocationInMap()
{
	return _location;
}

vector<Node*> Node::GetAdjacentList()
{
	return _adj;
}

void Node::AddNeighbor(Node *neighbor)
{
	_adj.push_back(neighbor);
}

NodeType Node::GetType() const{
	return _type;
}

Node::Node(Position *location, NodeType type) :_type (type){
	_location = location;
	_initialCost = 0;
	_g = -1;
	_h = -1;
}

Node::~Node() {
	// TODO Auto-generated destructor stub
}

