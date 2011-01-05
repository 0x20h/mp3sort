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
	class Dispatcher {
		public:
			Dispatcher(Config::Options& options, int size) : options(options), size(size), t_queue(size) {}
			~Dispatcher();
			void dispatch(std::string work);
			void join();
			Handler* getHandler(const std::string& type);
		private:
			std::map<std::string,Document::Handler *(*)()> m_factories;
			std::map<std::string,Document::Handler *> m_handlers;
			Thread::Blocking::Queue<boost::thread*> t_queue;
			boost::thread_group t_group;
			int size;
			Config::Options options;
	};
}

#endif // DOCUMENT_DISPATCHER_H
