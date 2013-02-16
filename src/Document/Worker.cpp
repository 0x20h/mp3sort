#include "Worker.h"
#include <boost/filesystem.hpp>

using namespace Document;
using namespace std;
using namespace boost::filesystem;

void Worker::operator ()() {
	while (true) {
		// get next item from queue
		string *item = queue->dequeue();
		
		// stop marker?
		if (*item == pp) {
			return;
		}

		Handler *h = dispatcher->getHandler(*item);
		Metadata m = h->getMetadata(*item);
		if (dispatcher->options.hasOption("verbose")) {
			cout <<  m.filename << endl;
		}

		if (!m.complete()) {
			cerr << "unable to retrieve complete Metadata, skipping " << *item << endl;
		} else {
			// copy
			if (!dispatcher->options.hasOption("metadata-only")) {
				string dst = m.resolve(dispatcher->options.getOption("dst").as<string>());	
				// add extension to dst
				dst += extension(*item);	
				path copy_from(*item);
				path copy_to(dst);
				
				if (!is_directory(copy_to.parent_path())) {
					create_directories(copy_to.parent_path());
				} 

				copy_file(copy_from, copy_to);

				// store metadata
				h->storeMetadata(dst, m);

				// move
				if (dispatcher->options.hasOption("move")) {
					// delete(*item)
				}
			}
		}
			
		delete h;
	}
}
