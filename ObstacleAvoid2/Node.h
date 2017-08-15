/*
 * Node.h
 *
 *  Created on: Jun 30, 2017
 *      Author: user
 */

#ifndef NODE_H_
#define NODE_H_

struct Node
{
	Node* parent;
	int row, col;
	float f, g, h;

	bool operator== (const Node* n1) const
	{
		return n1->row == row && n1->col == col;
	}
};

#endif /* NODE_H_ */
