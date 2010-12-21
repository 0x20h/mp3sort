#include "../Handler.h"

namespace Document 
{
	class MP3 : public Handler {
		public:
			MP3();
			~MP3();
		protected:
			void process(std::string *path);
	};
}
