#ifndef POSITION_H_
#define POSITION_H_

class Position {
private:
	double _yaw;
	double _x;
	double _y;
public:
	double Yaw();
	double X();
	double Y();
	void Update(double x, double y, double yaw = 0);
	Position(double x, double y, double yaw = 0);
	virtual ~Position();
};

#endif /* POSITION_H_ */
