#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include "Position.h"

using namespace std;

enum NodeType{
	PARTICLE,
	MAX_PARTICLE,
	PATH,
	WAYPOINT,
	ROBOT,
	PATH_START_POINT,
	PATH_END_POINT
};

class Node {
private:
	double _initialCost;
	// Approximate cost to the target(heuristic function)
	double _h;
	// Cost from the beginning
	double _g;
	Position *_location;
	vector<Node*> _adj;
	const NodeType _type;

public:
	void SetH(double h);
	void SetG(double g);
	void SetInitialCost(double cost);
	void AddNeighbor(Node *neighbor);

	double GetH();
	double GetG();
	double GetF();
	double GetInitialCost();
	NodeType GetType() const;
	Position* GetLocationInMap();
	vector<Node*> GetAdjacentList();

	Node(Position *location, NodeType type);
	virtual ~Node();
};

#endif /* NODE_H_ */
