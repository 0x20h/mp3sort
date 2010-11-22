#include <boost/thread.hpp>
#include <queue>
#include <iostream>
#ifndef STRUCT_BLOCKING_QUEUE_H
#define STRUCT_BLOCKING_QUEUE_H

namespace Thread { namespace Blocking {

template <class T> class Queue
{
	public:
		Queue(int N);
		~Queue();
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
		// condition var for empty/full queue
		boost::condition_variable filled, emptied;
};

template <class T>
Queue<T>::Queue(int N) {
	this->N = N;
}

template <class T>
Queue<T>::~Queue() {
	N = 0;
}

template <class T>
void Queue<T>::enqueue(T element) {
	boost::unique_lock<boost::mutex> lock(mutex);

	while(q.size() == N) {
		emptied.wait(lock);
	}

	q.push(element);
	filled.notify_one();
}

template <class T> 
T Queue<T>::dequeue() {
	boost::unique_lock<boost::mutex> lock(mutex);

	while (q.size() == 0) {
		filled.wait(lock);
	}
	
	T el = q.front();
	q.pop();
	emptied.notify_one();
	return el;
}

template <class T>
int Queue<T>::size() {
	return q.size();
}

template <class T>
bool Queue<T>::empty() {
	return q.empty();
}

}}
#endif // STRUCT_BLOCKING_QUEUE_H
