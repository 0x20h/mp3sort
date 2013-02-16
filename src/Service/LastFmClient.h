#include "../../deps/0x20h_Fingerprinter/src/fpclient/HTTPClient.h"
#include "../Document/Metadata.h"
#include "../Config/Options.h"
#include "../../deps/tinyxml2/tinyxml2.h"

#ifndef MP3SORT_LASTFM_CLIENT_H__

namespace Service {
	class LastFmClient {
		public:
 	 		LastFmClient(Config::Options *options): options(options) {}
 	 		~LastFmClient();
            tinyxml2::XMLDocument* trackGetInfo(std::string mbid);
			tinyxml2::XMLDocument* albumGetInfo(std::string mbid);
			tinyxml2::XMLDocument* albumGetInfo(std::string artist, std::string album);
			std::string artistGetInfo(std::string mbid);
			int getFingerprint(std::pair<const char*, size_t> fpData, std::map<std::string, std::string> params);
            tinyxml2::XMLDocument* fingerprintGetMetadata(int fpid);
            void getMetadata(int fpid, Document::Metadata* meta);
            void print(tinyxml2::XMLDocument* doc);
		protected:
			HTTPClient client;
			Config::Options *options;
	};
};
#endif
