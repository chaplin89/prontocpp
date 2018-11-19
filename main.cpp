#include "configuration.hpp"
#include "logging.hpp"

int main(int argc, const char *argv[])
{
	Configuration config;
	config.LoadConfig(argc, argv);
	auto main_config = config.GetConfig();
	assert(main_config.sampleconfig() == 1);
	return 0;
}