#include "../Handler.h"

namespace Document 
{
	class Ogg : public Handler {
		public:
			std::string getDescription();
			Metadata process(const std::string filename);
	};
}
