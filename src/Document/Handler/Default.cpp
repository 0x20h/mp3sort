#include "Default.h"
#include <iostream>

using namespace Document;

Metadata Default::process(std::string filename) {
	Metadata m;
//	std::cout << "thread " << boost::this_thread::get_id() << " doing nothing with " << std::endl;
	return m;
}

std::string Default::getDescription() {
	return "Default Handler";
}
