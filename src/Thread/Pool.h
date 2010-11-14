#include <boost/thread.hpp>
#include <string>
#ifndef STRUCT_BLOCKING_QUEUE_H
#include "../Struct/BlockingQueue.h"
#endif

namespace mp3sort
{
	namespace Thread 
	{
		class Pool {
			public:
				Pool(int size);
				~Pool();
				int size();
				boost::thread * get();
				static Pool create(int size);
			private:
				std::list<boost::thread *> pool;
				mp3sort::Struct::BlockingQueue<std::string> queue (int size);
		};
	}
}
