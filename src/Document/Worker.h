#include "Handler.h"
#include "Dispatcher.h"
#include "../Thread/Blocking/Queue.h"

#ifndef DOCUMENT_WORKER_H
#define DOCUMENT_WORKER_H

namespace Document 
{
	/**
 	* stop marker
 	*/
	static std::string pp ("-");

	class Worker {
		public:
			Thread::Blocking::Queue<std::string *> *queue;
			Dispatcher *dispatcher;
			void operator ()();
	};
}

#endif // DOCUMENT_WORKER_H
