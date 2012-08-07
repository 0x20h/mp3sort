#include "Handler.h"
#include "../Thread/Blocking/Queue.h"

using namespace Document;

void Handler::setOptions(const Config::Options *o) {
	this->options = o;
}
