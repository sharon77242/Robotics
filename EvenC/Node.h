/*
 * Node.h
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#ifndef NODE_H_
#define NODE_H_

struct Node {
	int row, col;
	float f, g, h;
	Node *parent;
};

#endif /* NODE_H_ */
