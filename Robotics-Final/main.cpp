#include "ParametersReader.h"
#include "RobotController.h"
const char* PARAMS_FILE = "parameters.txt";

int main() {
	Hamster* hamster;
	try {
		ParametersReader* reader = new ParametersReader(PARAMS_FILE);
		hamster = new HamsterAPI::Hamster(1);
		sleep(3);
		RobotController* controller = new RobotController(hamster, reader);
		controller->Start();
		cout << "Finish. " << endl;
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what()); }

	return 0;
}
