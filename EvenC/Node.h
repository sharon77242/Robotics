/*
 * Node.h
 *
 *  Created on: May 9, 2017
 *      Author: user
 */

#ifndef NODE_H_
#define NODE_H_

class Node {
public :
	int row, col;
	float f, g, h;
	Node *parent;

    inline bool operator == (const Node &b) const
    {
        return (b.row==row) && (b.col==col);
    }
};

#endif /* NODE_H_ */
