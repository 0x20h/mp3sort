#include "../Handler.h"

namespace Document 
{
	class Default : public Handler {
		public:
			Metadata process(const std::string filename);
			std::string getDescription();
	};
}
