/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef NODE_H_
#define NODE_H_

#include "Vector2d.h"

class Node {
private:
	Vector2d _vector;
	float _g; // The cost until this search cell g(x)
	float _h; // The expected cost to the goal h(x)
	float _turnFactor;
	float _wallFactor;

public :
	Node();
	Node(Vector2d v, float GGrade);

	Vector2d getVector();
	void setVector(Vector2d vector2d);
	float getG();
	void setG(float g);
	float getH();
	float getF();
	float getTurnFactor();
	void setTurnFactor(float turnFactor);
	float getWallFactor();
	void setWallFactor(float wallFactor);

	void calcHGrade(Vector2d goal);
};

#endif
