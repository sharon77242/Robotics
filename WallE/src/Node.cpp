/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "Node.h"

Node::Node() {
	_vector = Vector2d();
	_g = 0;
	_h = 0;
	_turnFactor = 0;
	_wallFactor = 0;
}

Node::Node(Vector2d v, float GGrade) {
	_vector = v;
	_g = GGrade;
	_h = 0;
	_turnFactor = 0;
	_wallFactor = 0;
}

Vector2d Node::getVector() {
	return _vector;
}

void Node::setVector(Vector2d vector2d) {
	_vector = vector2d;
}

float Node::getG() {
	return _g;
}

void Node::setG(float g) {
	_g = g;
}

float Node::getH() {
	return _h;
}

float Node::getF() {
	return _g + _h;
}

float Node::getTurnFactor() {
	return _turnFactor;
}

void Node::setTurnFactor(float turnFactor) {
	_turnFactor = turnFactor;
}

float Node::getWallFactor() {
	return _wallFactor;
}

void Node::setWallFactor(float wallFactor) {
	_wallFactor = wallFactor;
}

void Node::calcHGrade(Vector2d goal) {
	_h = _vector.manhattanDistance(goal) * 2;
}

