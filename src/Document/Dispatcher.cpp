#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include "Dispatcher.h"
#include "Handler/Default.h"

using namespace Document;

Dispatcher::~Dispatcher() {
	// kill all handler threads and delete references
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

struct ThreadExit {
	Handler *worker;

	void operator()() const {
		std::cout << boost::this_thread::get_id() << " exiting" << std::endl;
//		delete worker;
	}
};


struct ThreadContext {
	Handler *worker;

	void operator()() const {
		std::cout << boost::this_thread::get_id() << " start" << std::endl;
		worker->process();
		std::cout << boost::this_thread::get_id() << " finished" << std::endl;
		delete worker;
		return;
	}
};

void Dispatcher::dispatch(std::string work) {
	ThreadContext context;
	// identify type of work
	std::string type = boost::filesystem::extension(work).erase(0,1);
	Handler* h = this->getHandler(type);
	h->setWork(work);
	h->setOptions(&options);
	context.worker = h;
	// add thread to pool
	t_group.create_thread(context);
}	
