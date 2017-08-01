/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include "Map.h"
#include "Node.h"
#include "Vector2d.h"
#include "AStarPredicate.h"
#include "WaypointsManager.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <tuple>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

class PathPlanner {
private:
	list<Node> openList;
	priority_queue<Node, vector<Node>, AStarPredicate> openQueue;
	list<Node> closedList;
	Vector2d startPosition;
	Vector2d endPosition;
	Map * _map;
	map<int, Vector2d> parentsMap;

	list<Node> getNeighbors(Node * node);
	list<Vector2d> reconstructPath(Vector2d endPosition);
	float calcDirectionFactor(Vector2d v1, Vector2d v2, Vector2d v3);
	float calcWallFactor(Vector2d vector2d, int wallDistance);

public:
	PathPlanner(Map * map, Vector2d startPosition, Vector2d endPosition);

	list<Vector2d> performAStar();
};

#endif
