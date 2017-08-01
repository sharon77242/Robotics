/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "PathPlanner.h"

using namespace std;

PathPlanner::PathPlanner(Map * map, Vector2d _startPosition, Vector2d _endPosition) {
	_map = map;
	startPosition = _startPosition;
	endPosition = _endPosition;
}

list<Vector2d> PathPlanner::performAStar() {
	Node startNode(startPosition, 0);
	startNode.calcHGrade(endPosition);

	map<int,bool> openMap;
	map<int,bool> closedMap;

	openQueue.push(startNode);
	openMap[startPosition.hashCode()] = true;

	while (!openQueue.empty()) {
		Node currMinNode = openQueue.top();

		if (currMinNode.getVector() == endPosition) {
			return reconstructPath(currMinNode.getVector());
		}

		openQueue.pop();
		openMap[currMinNode.getVector().hashCode()] = false;
		closedMap[currMinNode.getVector().hashCode()] = true;

		list<Node> neighbors = getNeighbors(&currMinNode);
		for (list<Node>::iterator nodesIterator = neighbors.begin(); nodesIterator != neighbors.end(); nodesIterator++) {
			Node *currNeighbor = nodesIterator.operator ->();

			if (!closedMap[currNeighbor->getVector().hashCode()]) {
				float tempNeighborGGrade = (currMinNode.getG() + 1) * 0.05 + currNeighbor->getTurnFactor() * 0.75 +
						currNeighbor->getWallFactor() * 0.2;

				if (!openMap[currNeighbor->getVector().hashCode()] || tempNeighborGGrade < currNeighbor->getG()) {
					parentsMap[currNeighbor->getVector().hashCode()] = currMinNode.getVector();
					currNeighbor->setG(tempNeighborGGrade);
					currNeighbor->calcHGrade(endPosition);

					if (!openMap[currNeighbor->getVector().hashCode()]) {
						openQueue.push(*currNeighbor);
						openMap[currNeighbor->getVector().hashCode()] = true;
					}
				}
			}
		}

		neighbors.clear();
	}

	return list<Vector2d>();
}

list<Node> PathPlanner::getNeighbors(Node *node) {
	list<Node> neighbors;

	for (int rowsIndex = node->getVector().getY() - 1; rowsIndex <= node->getVector().getY() + 1; rowsIndex++) {
		if (!(rowsIndex < 0 || rowsIndex >= _map->getHeight())) {
			for (int columnsIndex = node->getVector().getX() - 1; columnsIndex <= node->getVector().getX() + 1; columnsIndex++) {
				if (!(columnsIndex < 0 || columnsIndex >= _map->getWidth())) {
					if ((_map->getCellValue(columnsIndex, rowsIndex, _map->getGridResolution()) == Map::FREE_CELL) &&
						!((node->getVector().getX() == columnsIndex) && (node->getVector().getY() == rowsIndex))) {

						Vector2d neighborPoint(columnsIndex, rowsIndex);
						Node neighbor(neighborPoint, std::numeric_limits<float>::max());

						if (node->getVector() != startPosition) {
							neighbor.setTurnFactor(calcDirectionFactor(parentsMap[node->getVector().hashCode()], node->getVector(), neighborPoint));
						}

						neighbor.setWallFactor(calcWallFactor(neighborPoint, 3));

						neighbors.push_back(neighbor);
					}
				}
			}
		}
	}

	return neighbors;
}

list<Vector2d> PathPlanner::reconstructPath(Vector2d endPosition) {
	list<Vector2d> path = list<Vector2d>();
	Vector2d tempPoint = endPosition;

	while (parentsMap[tempPoint.hashCode()] != startPosition) {
		path.push_front(tempPoint);
		tempPoint = parentsMap[tempPoint.hashCode()];
	}

	path.push_front(tempPoint);
	path.push_front(parentsMap[tempPoint.hashCode()]);

	return path;
}

float PathPlanner::calcDirectionFactor(Vector2d v1, Vector2d v2, Vector2d v3) {
	WaypointsManager::Direction firstDirection = WaypointsManager::getDirection(v1,v2);
	WaypointsManager::Direction secondDirection = WaypointsManager::getDirection(v2,v3);

	int difference = abs(secondDirection - firstDirection) % 8;

	if (difference == 0) {
		return 0;
	}
	if (difference == 1 || difference == 7) {
		return 2;
	}
	if (difference == 2 || difference == 6) {
		return 4;
	}
	if (difference == 3 || difference == 5) {
		return 8;
	}
	if (difference == 4) {
		return 30;
	}
}

float PathPlanner::calcWallFactor(Vector2d vector2d, int wallDistance) {
	int wallCounter = 0;
	for (int rowsIndex = vector2d.getY() - wallDistance; rowsIndex <= vector2d.getY() + wallDistance; rowsIndex++) {
		if (!(rowsIndex < 0 || rowsIndex >= _map->getHeight())) {
			for (int columnsIndex = vector2d.getX() - wallDistance; columnsIndex <= vector2d.getX() + wallDistance; columnsIndex++) {
				if (!(columnsIndex < 0 || columnsIndex >= _map->getWidth())) {
					if (_map->getCellValue(columnsIndex, rowsIndex, _map->getGridResolution()) == Map::OCCUPIED_CELL) {
						wallCounter++;
					}
				}
			}
		}
	}

	return wallCounter * 5;
}
