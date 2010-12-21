#include "MP3.h"
#include <iostream>
#include <boost/random.hpp>

boost::minstd_rand gen(42u);

using namespace Document;

MP3::MP3() {}
MP3::~MP3() {}

void MP3::process(std::string *path) {
	boost::uniform_int<> uni_dist(0,4000);
	boost::variate_generator<boost::minstd_rand&, boost::uniform_int<> > uni(gen, uni_dist);
	int sleep = uni();
	std::cout << "thread " << boost::this_thread::get_id() << " sleeping for " << sleep << std::endl;
	boost::posix_time::milliseconds s(sleep);
	boost::this_thread::sleep(s);
	onFinishCallback(this);
}


extern "C" {
	Handler* factory() { return new MP3; } 
}
