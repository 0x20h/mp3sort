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
			void start(std::string path, void (*onFinish) (Document::Handler*)) {
				onFinishCallback = onFinish;
				this->a_thread = boost::thread(&Handler::process, this, &path);
			}
			
			void join() {
				a_thread.join();
			}
			
			void setOptions(Config::Options *options) {
				this->options = options;
			}
			
		protected:
			virtual void process(std::string *path) = 0;
			void (*onFinishCallback) (Document::Handler*);
			
		private:
			boost::thread a_thread;
			Config::Options *options;
	};
}

#endif //DOCUMENT_HANDLER_H
