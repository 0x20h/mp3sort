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
		m.interpret = "foo";
		m.album = boost::filesystem::basename(*item);
		m.genre = "Rock";
		m.title = "A Song";
		m.track_no = 3;
		m.year = 2012;

		if (!m.complete()) {
			cerr << "unable to retrieve Metadata, skipping " << *item << endl;
		} else {
			// store metadata
			h->storeMetadata(*item, m);

			// copy
			if (dispatcher->options.getOption("dst").as<string>() != "") {

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
