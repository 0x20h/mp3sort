#include <iostream>
#include "Config/Options.h"

int main(int argc, char* argv[]) {
	Config::Options options = Config::Options(argc, argv);

	if (argc < 2 || options.hasOption("help")) {
		std::cout << options.getDescription() << std::endl;
		return 0;
	}
	
	if (options.hasOption("move")) {
		std::cout << "moving files\n";
	}
	return 0;
}

