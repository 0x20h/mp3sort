#include "../Handler.h"

namespace Document 
{
	class Ogg : public Handler {
		public:
			Ogg();
			~Ogg();
		protected:
			void process(std::string *path);
	};
}
