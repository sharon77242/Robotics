#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include "Position.h"

enum NodeType{
	PARTICLE,
	MAX_PARTICLE,
	PATH,
	WAYPOINT,
	ROBOT,
	PATH_START_POINT,
	PATH_END_POINT
};

struct Node {
	Node(Position location, NodeType type);

	void addNeighbor(Node *neighbor);
	double calculateF();

	double initialCost;
	// Approximate cost to the target(heuristic function)
	double h;
	// Cost from the beginning
	double g;
	Position location;
	std::vector<Node*> adj;
	const NodeType type;

};

#endif /* NODE_H_ */
