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
			Dispatcher(const Config::Options& options, const int size) : options(options), size(size), t_queue(size) {}
			~Dispatcher();
			void dispatch(std::string work);
			void init();
			void join();
		private:
			Handler* getHandler(const std::string& type);
			std::map<std::string,Document::Handler *(*)()> m_factories;
			std::map<std::string,Document::Handler *> m_handlers;
			Thread::Blocking::Queue<std::string *> t_queue;
			boost::thread_group t_group;
			int size;
			int running;
			Config::Options options;
	};
}

#endif // DOCUMENT_DISPATCHER_H
