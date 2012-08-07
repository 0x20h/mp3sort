#include "Default.h"
#include <iostream>

using namespace Document;

void Default::storeMetadata(const std::string& filename, const Metadata& metadata) {
}

Metadata Default::getMetadata(const std::string& filename) {
	Metadata m;
	std::cout << "thread " << boost::this_thread::get_id() << " doing nothing with " << filename << std::endl;
	return m;
}
