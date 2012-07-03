#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include "../Config/Options.h"
#include "../Thread/Blocking/Queue.h"

namespace Document 
{
	class Handler {
		public:
			Handler();
			~Handler();
			void setOptions(const Config::Options *o);
			void setQueue(Thread::Blocking::Queue<std::string *> *q);
			virtual void process() = 0;
			virtual std::string getDescription() = 0;
		protected:
			const Config::Options *options;
			Thread::Blocking::Queue<std::string *> *queue;
	};
}

#endif //DOCUMENT_HANDLER_H
