#include "Ogg.h"
#include <iostream>

using namespace Document;

void Ogg::process() {
	std::cout << "thread " << boost::this_thread::get_id() << " handling gg"  << std::endl;
}

std::string Ogg::getDescription() {
	return "Ogg handler";
}

extern "C" {
	Handler* factory() { return new Ogg; } 
}
