#include "../../deps/Fingerprinter/src/fpclient/HTTPClient.h"
#include "../Document/Metadata.h"

#ifndef MP3SORT_LASTFM_CLIENT_H__

namespace Service {
	class LastFmClient {
		public:
 	 		LastFmClient();
 	 		~LastFmClient();
			std::string trackGetInfo(std::string mbid);
			std::string albumGetInfo(std::string mbid);
			std::string artistGetInfo(std::string mbid);
			int getFingerprint(std::pair<const char*, size_t> fpData, std::map<std::string, std::string> params);
			std::string fingerprintGetMetadata(int fpid);
            void getMetadata(int fpid, Document::Metadata* meta);
		protected:
			HTTPClient client;	
	};
};
#endif
