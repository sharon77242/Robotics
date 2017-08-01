/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#include "AStarPredicate.h"

AStarPredicate::AStarPredicate() {

}

bool AStarPredicate::operator() (Node first, Node second) const {
	return first.getF() > second.getF();
}
