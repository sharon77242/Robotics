#include "RobotController.h"
#include <string.h>
#include "ConfigurationManager.h"

int main() {
	const string PARAMS_FILE = "parameters.txt";
	try {
		ConfigurationManager* reader = new ConfigurationManager(PARAMS_FILE);
		Hamster hamster(1);
		sleep(3);
		RobotController* controller = new RobotController(hamster, reader);
		controller->Start();
		cout << "Finish. " << endl;

	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what()); }

	return 0;
}
