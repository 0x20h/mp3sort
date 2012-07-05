#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "Dispatcher.h"
#include "Handler/Default.h"

using namespace Document;

struct Worker {
	Thread::Blocking::Queue<std::string *> *queue;
	Dispatcher *dispatcher;

	void operator ()();
};

void Worker::operator ()() {

		while (true) {
			// get next item from queue
			std::string *item = queue->dequeue();
			std::cout << boost::this_thread::get_id() <<" starts processing " << item << std::endl;

			boost::filesystem::path p(*item);
			std::string type = boost::filesystem::extension(p).erase(0,1);
			Handler *h = dispatcher->getHandler(type);
			h->process();
			std::cout << boost::this_thread::get_id() << " finished" << std::endl;
		}

//		shazaan 
//		soundhound
	return;
}

Dispatcher::~Dispatcher() {
	// kill all handler threads and delete references
}

void Dispatcher::init() {
	Worker *w;

	for (int i = 0; i < 4; i++) {
		std::cout << boost::this_thread::get_id() << " initializing worker " << i << std::endl;
		w = new Worker;
		w->queue = &this->t_queue;
		w->dispatcher = this;
		t_group.create_thread(*w);
	}
}

Handler* Dispatcher::getHandler(const std::string& type) {
	std::map<std::string, Document::Handler *(*)()>::iterator it;
	Handler *h;
	it = m_factories.find(type);

	if (it == m_factories.end()) {	
		// factory not initialized, try to load handler lib
		std::string lib ("lib/libhandler-");
		lib.append(type).append(".so");
		void *handle = dlopen(lib.c_str(), RTLD_LAZY);
		if (NULL == handle) {
			// return default handler
			std::cout << "Error loading: " << dlerror() << std::endl;
			// associate default handler with the type
			m_handlers[type] = new Default;
			return new Default;
		} else {
			m_factories[type] = (Document::Handler *(*)()) dlsym(handle, "factory");
			m_handlers[type] = m_factories[type]();
		}
	}

//	return m_handlers[type];	
	return m_factories[type]();
}

void Dispatcher::join() {
	t_group.join_all();
}

void Dispatcher::dispatch(std::string work) {
	t_queue.enqueue(&work);
}
