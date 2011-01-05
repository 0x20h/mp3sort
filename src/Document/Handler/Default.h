#include "../Handler.h"

namespace Document 
{
	class Default : public Handler {
		public:
			void process();
			std::string getDescription();
	};
}
