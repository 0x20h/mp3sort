#include "../Handler.h"

namespace Document 
{
	class Ogg : public Handler {
		public:
			std::string getDescription();
			void process();
	};
}
