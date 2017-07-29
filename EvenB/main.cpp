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
	sleep(2);

	OccupancyGrid grid = hamster->getSLAMMap();
	Map map(grid);

	while (hamster->isConnected()) {
		map.show();
		sleep(0.5);
	}

	delete hamster;

	return 0;
}



