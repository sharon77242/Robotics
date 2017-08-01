/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef ASTARPREDICATE_H_
#define ASTARPREDICATE_H_

#include "Node.h"

using namespace std;

class AStarPredicate : public std::binary_function<Node, Node, bool>{
public:
	AStarPredicate();

	bool operator() (Node first, Node second) const;
};

#endif
