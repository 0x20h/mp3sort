#include <boost/thread.hpp>
#include <queue>
#ifndef STRUCT_BLOCKING_QUEUE_H
#define STRUCT_BLOCKING_QUEUE_H
#endif

namespace mp3sort
{

namespace Struct 
{

template <class T> class BlockingQueue
{
	public:
		BlockingQueue(int N);
		~BlockingQueue();
		void enqueue(T element);
		T dequeue();
		int size();
		bool empty();
	private:
		// internal queue
		std::queue<T> q;
		// queue size
		int N;
		// mutex
		boost::mutex mutex;
};

template <class T>
BlockingQueue<T>::BlockingQueue(int N) {
	this->N = N;
}

template <class T>
BlockingQueue<T>::~BlockingQueue() {
	this->N = 0;
}

template <class T>
void BlockingQueue<T>::enqueue(T element) {
	// aquire lock
	if(q.size() < N) {
		q.push(element);
	}
	// release lock
}

template <class T>
T BlockingQueue<T>::dequeue() {
	// acquire lock
	T el = q.front();
	q.pop();
	// release lock
	return el;
}

template <class T>
int BlockingQueue<T>::size() {
	return q.size();
}

template <class T>
bool BlockingQueue<T>::empty() {
	return q.empty();
}

}
}
