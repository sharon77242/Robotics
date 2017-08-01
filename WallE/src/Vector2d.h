
#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <string>
#include <cmath>
#include <sstream>

using namespace std;

class Vector2d {
private:
	int _x;
	int _y;
public:
	Vector2d();
	Vector2d(int x, int y);

	int getX();
	void setX(int x);
	int getY();
	void setY(int y);

	float distanceBetweenVector(Vector2d vector2d);
	float manhattanDistance(Vector2d vector2d);
	float diagonalDistance(Vector2d vector2d);
	float squaredEuclideanDistance(Vector2d vector2d);

	int hashCode();
	string toString();

	bool operator==(const Vector2d & vector2d) const;
	bool operator!=(const Vector2d & vector2d) const;
	bool operator<(const Vector2d & vector2d) const;
	bool operator>(const Vector2d & vector2d) const;
	bool operator<=(const Vector2d & vector2d) const;
	bool operator>=(const Vector2d & vector2d) const;
	Vector2d operator+(const Vector2d & vector2d) const;
	Vector2d operator-(const Vector2d & vector2d) const;
	Vector2d operator+(const int & number) const;
	Vector2d operator-(const int & number) const;
	Vector2d operator/(const int & number) const;
	Vector2d operator/(float & number) const;
	Vector2d operator*(const int & number) const;
	void operator=(const Vector2d & vector2d);
};

#endif
