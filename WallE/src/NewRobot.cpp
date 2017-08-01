//============================================================================
// Name        : WallE.cpp
// Author      :
// Version     :
// Copyright   :
// Description :
//============================================================================
// Ofri Sherf         312481112
// Maayan Angel       312115256
// May Stotzky Elkin  312349285

#include "Brain.h"
#include "Robot.h"

using namespace std;
using namespace PlayerCc;

int main()
{
//	Robot WallE("10.10.245.63", 6665);
	Robot WallE("localhost", 6665);
	Brain* brain = new Brain(&WallE);
	brain->run();

	return 0;
}
