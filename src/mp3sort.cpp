#include <iostream>
#include <stdio.h>
#include <boost/thread.hpp>
#include <dlfcn.h>
#include "Config/Options.h"
#include "Thread/Blocking/Queue.h"
#include "Document/Handler.h"
#define HANDLERS 10
#define ELEMENTS 500

using namespace std;

Thread::Blocking::Queue<string> work(4);
Thread::Blocking::Queue<Document::Handler*> freeQueue(HANDLERS);
// store handlers in array for joining 
Document::Handler* h_list[HANDLERS];

bool writer_finished = 0;

void writer(const Config::Options * options) {
	int i=0;
	string item = "foobar";

	for (int i=0; i < ELEMENTS; i++) {
//		cout << boost::this_thread::get_id() << " enqueuing " << item << endl;
		work.enqueue(item);
	}

	writer_finished = 1;
}

/**
 * callback is called when worker threads have finished processing
 */
void onFinish(Document::Handler *handler) {
	cout << "readding to freelist: " << boost::this_thread::get_id() <<endl;
	freeQueue.enqueue(handler);
}

void reader() {
	void *handle = dlopen("lib/libhandler-ogg.so", RTLD_LAZY);

	if (NULL == handle) {
		std::cout << "Error loading: " << dlerror() << std::endl;
	} 
	
	Document::Handler *(*factory)() = (Document::Handler *(*)()) dlsym(handle, "factory");
	Document::Handler *handler;
	for (int i=0; i < HANDLERS; i++) {
		cout << "initiating handler #" << i << endl;
		h_list[i] = (*factory)();
		freeQueue.enqueue(h_list[i]);
	}

	cout << "handlers initiated in " << boost::this_thread::get_id() << endl;	
	Document::Handler *h;

	// process work tasks until writer has finished and queue is empty
	while (!writer_finished || !work.empty()) {
		string val = work.dequeue();
//		cout << "read " << val << " from workqueue " << endl;
		h = freeQueue.dequeue();
		h->start(val, &onFinish);
	}
	
	// wait until all handler threads arrive back in handlerQueue
	for (int i=0; i < HANDLERS; i++) {
		h_list[i]->join();
	}
			
//	cout << "finished processing..." << endl;	
}

int main(int argc, char* argv[]) {
	try {
		Config::Options options = Config::Options(argc, argv);
		
		if (argc < 2 || options.hasOption("help")) {
			std::cout << options.getDescription() << std::endl;
			return 0;
		}
		
		if (!options.hasOption("src")) {
			std::cerr << "ERROR: please specify source directory" << std::endl;
			std::cout << options.getDescription() << std::endl;
			return 1;
		}

		boost::thread w = boost::thread(&writer, &options);
		boost::thread r = boost::thread(&reader);
		// reader must take quite longer than writer, so join reader only
		r.join();
		return 0;	
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}
