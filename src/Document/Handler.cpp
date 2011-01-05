#include "Handler.h"

using namespace Document;

Handler::Handler() {}
Handler::~Handler() {}

void Handler::setWork(std::string file) {
	this->work = file;
}

void Handler::setOptions(const Config::Options *o) {
	this->options = o;
}


