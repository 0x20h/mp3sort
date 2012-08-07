#include "Metadata.h"
#include <iostream>

/**
 * check if all properties are not empty
 *
 * @return bool true if all properties are not empty
 */
bool Document::Metadata::complete() {
	if (
		filename == "" ||
		interpret == "" ||
		title == "" ||
		album == "" ||
		genre == "" ||
		track_no < 1 ||
		year < 1
	) {

		return false;
	}

	return true;
}


std::string Document::Metadata::resolve(std::string pattern) {
	int pos;

	// interpret
	if ((pos = pattern.find("%i")) >= 0) {
		pattern.replace(pos, 2, interpret);
	}

	// album
	if ((pos = pattern.find("%a")) >= 0) {
		pattern.replace(pos, 2, album);
	}

	// song title
	if ((pos = pattern.find("%s")) >= 0) {
		pattern.replace(pos, 2, title);
	}

	// track no
	if ((pos = pattern.find("%n")) >= 0) {
		pattern.replace(pos, 2, "3");
	}
	
	return pattern;
}
