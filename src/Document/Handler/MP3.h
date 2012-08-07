#include "../Handler.h"

namespace Document 
{
	class MP3 : public Handler {
		public:
			Metadata getMetadata(const std::string& filename);
			void storeMetadata(const std::string& filename, const Metadata& metadata);
		protected:
			bool readID3v2(const std::string& filename, Metadata& meta);
	};
}
