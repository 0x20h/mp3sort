#ifndef DOCUMENT_HANDLER_H
#define DOCUMENT_HANDLER_H

#include <boost/thread.hpp>
#include <string>
#include <iostream>

#include "Metadata.h"
#include "../Config/Options.h"
#include "../Thread/Blocking/Queue.h"

namespace Document 
{
	class Handler {
		public:
			void setOptions(Config::Options *o);
			/**
			 * retrieve Metadata from file, either from some tags or from a fp service...
			 */
			virtual Metadata getMetadata(const std::string& filename) = 0;

			/**
			 * store Metadata
			 */
			virtual void storeMetadata(const std::string& filename, const Metadata& metadata) = 0;
		protected:
			Config::Options *options;
	};
}

#endif //DOCUMENT_HANDLER_H
