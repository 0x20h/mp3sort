#include "../Handler.h"

namespace Document 
{
	class Default : public Handler {
		public:
			Metadata getMetadata(const std::string& filename);
			void storeMetadata(const std::string& filename, const Metadata& metadata);
	};
}
