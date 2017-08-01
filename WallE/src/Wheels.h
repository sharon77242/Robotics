/*
 *	Authors:
 *      Noam Okman : 307888560
 *      Omri Litov : 311248330
 *      Elad Bezalel: 204412803
 */

#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include "Robot.h"
#include "LocalizationManager.h"

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Wheels {
protected:
	bool _isStuck;
	Robot *_robot;
	LocalizationManager* _localizationManager;
	Vector2d _goalPoint;

	virtual void behave() = 0;

private:
	list<Wheels*> _Wheels;

public:
    Wheels(Robot * robot, LocalizationManager * localizationManager, Vector2d goalPoint);
    virtual ~Wheels();

    bool isStuck();

    virtual bool startCondition() = 0;
    virtual bool stopCondition() = 0;

    void action();
    void addNext(Wheels * Wheels);
    list<Wheels*> getWheelss();
};

#endif
