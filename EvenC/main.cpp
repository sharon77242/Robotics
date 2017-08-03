/*
 * Reading map and displaying it
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "Map.h"

using namespace std;
HamsterAPI::Hamster * hamster;
using namespace HamsterAPI;

int main() {
	hamster = new Hamster(1);
	sleep(1);

	OccupancyGrid grid = hamster->getSLAMMap();
	Pose startPos(5,5);
	Pose endPos(10,10);
	Map map(grid, startPos, endPos, 0.25);

	while (hamster->isConnected()) {
		map.show();
		sleep(0.2);
	}

	delete hamster;

	return 0;
}



