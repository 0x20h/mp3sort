#include "Pool.h"

using namespace mp3sort::Thread;

Pool::Pool(int size) {
	boost::thread * t;
	std::list<boost::thread *>::iterator iter = this->pool.begin();
	for(int i=0; i<size;i++) {
		t = new boost::thread;
		this->pool.insert(iter++,t);
	}
}

Pool::~Pool() {
}

int Pool::size() {
	return (int) this->pool.size();
}

boost::thread * Pool::get() {
	if( (int) this->pool.size() > 0 ) {	
		boost::thread * t = *this->pool.end();
		this->pool.pop_back();
		return t;
	}
}

Pool Pool::create(int size) {
	return Pool (size);
}
