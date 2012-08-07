#ifndef DOCUMENT_METADATA_H
#define DOCUMENT_METADATA_H

#include <string>

namespace Document 
{
	class Metadata {
		public:
			Metadata(): track_no(0), year(0) {}
			/**
			 * check if every property is set with a sane value
			 *
			 * @return bool true if metadata seems to be set
			 */
			bool complete();

			/**
			 * replace parameters in the given pattern with the metadata properties.
			 *
			 * The following replacement parameters are available:
			 * - %i Interpret
			 * - %a Album
			 * - %s Song Title
			 * - %t Track no
			 * - %g Genre
			 * - %y Year
			 */
			std::string resolve(std::string pattern);

			/* properties */
			std::string filename;
			std::string interpret;
			std::string title;
			std::string album;
			std::string genre;
			unsigned int track_no;
			unsigned int year;
	};
}

#endif //DOCUMENT_METADATA_H
