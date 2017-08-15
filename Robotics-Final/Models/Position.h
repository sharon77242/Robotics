#ifndef POSITION_H_
#define POSITION_H_

class Position {
public:
	Position() = default;
	Position(double x, double y, double heading = 0);
	double Heading();
	double X();
	double Y();
	void Update(double x, double y, double heading = 0);
private:
	double Heading_ = 0;
	double x_ = 0;
	double y_ = 0;
};

#endif /* POSITION_H_ */
