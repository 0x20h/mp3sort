#include "Default.h"
#include <iostream>

using namespace Document;

void Default::process() {
	std::cout << "thread " << boost::this_thread::get_id() << " doing nothing with " << std::endl;
}

std::string Default::getDescription() {
	return "Default Handler";
}
