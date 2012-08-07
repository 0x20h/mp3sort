#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "Dispatcher.h"
#include "Worker.h"
#include "Handler/Default.h"
#include "Handler/MP3.h"

using namespace Document;

Dispatcher::~Dispatcher() {
}

void Dispatcher::init() {
	Worker *w;

	for (int i = 0; i < num_workers; i++) {
		w = new Worker;
		w->queue = &this->t_queue;
		w->dispatcher = this;
		t_group.create_thread(*w);
	}
}

Handler* Dispatcher::getHandler(const std::string& item) {
	Handler *h;
	boost::filesystem::path p(item);
	std::string type = boost::filesystem::extension(p).erase(0,1);
	
	for (int i = 0; i < type.length(); i++) {
		type[i] = std::tolower(type[i]);
	}

	if (type == "mp3") {
		h = new MP3;
	} else {
		h = new Default;
	}

	h->setOptions(&options);
	return h;
}

/**
 * blocks this thread until all workers have finished all
 * pending jobs. 
 */
void Dispatcher::join() {
	for (int i = 0; i < num_workers; i++) {
		// add poison pill for each worker
		t_queue.enqueue(&pp);
	}

	t_group.join_all();
}

void Dispatcher::dispatch(const std::string& work) {
	t_queue.enqueue(new std::string(work));
}
