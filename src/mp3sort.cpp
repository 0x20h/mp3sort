#include <iostream>
#include "Config/Options.h"
#include "Thread/Blocking/Queue.h"

Thread::Blocking::Queue<int> q(400);

void observer() {
	while(true) {	
		std::cout << q.size() << std::endl;
		boost::this_thread::sleep(boost::posix_time::seconds (2));
	}	
}

void writer() {
	int i=0;
	while(true) {
		q.enqueue(i);
	}
}

void reader() {
	while(true) {
		int i = q.dequeue();
	}

}

int main(int argc, char* argv[]) {
	try {
		Config::Options options = Config::Options(argc, argv);

		if (argc < 2 || options.hasOption("help")) {
			std::cout << options.getDescription() << std::endl;
			return 0;
		}
	
		boost::thread t  = boost::thread(&writer);
		boost::thread t2  = boost::thread(&reader);
		boost::thread t3  = boost::thread(&observer);
		t.join();
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}

