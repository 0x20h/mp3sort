#include "Metadata.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

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
		track_no < 1
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
	if ((pos = pattern.find("%t")) >= 0) {
		std::ostringstream ss;
		ss << track_no;
		pattern.replace(pos, 2, ss.str());
	}
	
	return pattern;
}


std::string Document::Metadata::toString() {
	std::ostringstream oss;
	oss << "File: " << filename << std::endl
		<< "Artist: " << interpret << std::endl
		<< "Title: " << title << std::endl
		<< "Track No: " << track_no << std::endl
		<< "Album: " << album << std::endl
		<< "Year: " << year  << std::endl
		<< "--------------------------" << std::endl;
	return oss.str();
}
