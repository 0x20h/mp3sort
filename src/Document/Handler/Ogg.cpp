#include "Ogg.h"
#include <iostream>
#include <boost/random.hpp>

boost::minstd_rand gen(42u);

struct callback {
	void operator()() const {
		std::cout << "exiting " << boost::this_thread::get_id() << std::endl;
	};
};


using namespace Document;

Ogg::Ogg() {}
Ogg::~Ogg() {}

void Ogg::process(std::string *path) {
	callback x;
//	boost::uniform_int<> uni_dist(0,4000);
//	boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > uni(gen, uni_dist);
//	int sleep = uni();
	int sleep = 0;
	std::cout << "thread " << boost::this_thread::get_id() << " sleeping for " << sleep << std::endl;
//	boost::posix_time::milliseconds s(sleep);
//	boost::this_thread::sleep(s);
	boost::this_thread::at_thread_exit(x);
	onFinishCallback(this);
}


extern "C" {
	Handler* factory() { return new Ogg; } 
}
