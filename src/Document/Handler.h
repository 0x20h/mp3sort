#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include "../Config/Options.h"

namespace Document 
{
	class Handler {
		public:
			Handler();
			~Handler();
			void setOptions(const Config::Options *o);
			void setWork(std::string file);

			virtual void process() = 0;
			virtual std::string getDescription() = 0;
		protected:
			const Config::Options *options;
			std::string work;
	};
}

#endif //DOCUMENT_HANDLER_H
