#include "RobotController.h"
#include <string.h>
#include "ConfigurationManager.h"

int main()
{
	try
	{
		static const string PARAMS_FILE = "configuration.txt";

		ConfigurationManager reader(PARAMS_FILE);

		Hamster hamster(1);
		sleep(2);

		RobotController controller(hamster, reader);
		controller.Start();
		cout << "Finish. " << endl;

	} catch (const HamsterAPI::HamsterError& connection_error)
	{
		HamsterAPI::Log::i("Client", connection_error.what());
	}

	return 0;
}
