#include "../Handler.h"
#include "../../fpclient/MP3_Source.h"
#include "../../fpclient/HTTPClient.h"
#include <fplib/FingerprintExtractor.h>

namespace Document 
{
	class MP3 : public Handler {
		public:
			MP3();
			~MP3();
			std::string getDescription();
			Metadata process(const std::string filename);
	};
}
