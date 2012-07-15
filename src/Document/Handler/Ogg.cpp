#include "Ogg.h"
#include <iostream>

using namespace Document;

Metadata Ogg::process(std::string filename) {
	Metadata m;
	std::cout << "thread " << boost::this_thread::get_id() << " handling gg"  << std::endl;
	return m;
}

std::string Ogg::getDescription() {
	return "Ogg handler";
}

extern "C" {
	Handler* factory() { return new Ogg; } 
}
