#include "Handler.h"
#include "../Thread/Blocking/Queue.h"

using namespace Document;

Handler::Handler() {}
Handler::~Handler() {}

void Handler::setOptions(const Config::Options *o) {
	this->options = o;
}
