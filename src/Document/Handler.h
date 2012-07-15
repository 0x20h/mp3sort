#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include "../Config/Options.h"
#include "../Thread/Blocking/Queue.h"

namespace Document 
{
	struct Metadata {
		std::string filename;
		std::string fingerprint;
		std::string interpret;
		std::string title;
		std::string album;
		short int   error;
	};

	class Handler {
		public:
			Handler();
			~Handler();
			void setOptions(const Config::Options *o);
			virtual Document::Metadata process(const std::string filename) = 0;
			virtual std::string getDescription() = 0;
		protected:
			const Config::Options *options;
			Thread::Blocking::Queue<std::string *> *queue;
	};
}

#endif //DOCUMENT_HANDLER_H
