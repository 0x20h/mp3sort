#include <map>
#include <dlfcn.h>
#include <string>
#include <boost/thread.hpp>
#include "Handler.h"
#include "../Thread/Blocking/Queue.h"
#ifndef DOCUMENT_DISPATCHER_H
#define DOCUMENT_DISPATCHER_H

namespace Document 
{
	const static int num_workers = boost::thread::hardware_concurrency();

	class Dispatcher {
		public:
			Dispatcher(const Config::Options& options) : options(options), t_queue(num_workers) {}
			~Dispatcher();
			Handler* getHandler(const std::string& item);
			void dispatch(const std::string& work);
			void init();
			void join();
			Config::Options options;
		private:
			std::map<std::string,Document::Handler *(*)()> m_factories;
			std::map<std::string,Document::Handler *> m_handlers;
			Thread::Blocking::Queue<std::string *> t_queue;
			boost::thread_group t_group;
	};
}

#endif // DOCUMENT_DISPATCHER_H
