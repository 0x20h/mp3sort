#include "Worker.h"
#include <boost/filesystem.hpp>

using namespace Document;
using namespace std;

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
		
		if (!m.complete()) {
			cerr << "unable to retrieve Metadata, skipping " << *item << endl;
		} else {
			// store metadata
			h->storeMetadata(*item, m);

			// copy
			if (dispatcher->options.hasOption("dst")) {
				string dst = m.resolve(dispatcher->options.getOption("dst").as<string>());

				cout << "copying " << *item << " to " << dst << endl;
				// move
				if (dispatcher->options.hasOption("move")) {
					// delete(*item)
				}
			}
		}
			
		delete h;
	}
}
