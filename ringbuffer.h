#pragma once

#include <stdexcept>
#include <unistd.h>
#include "sequencer.h"
#include "util.h"

using namespace std;

template <typename T>
class RingBuffer {
public:
	RingBuffer(Sequencer *sequencer);
	T* get(long sequence);
	long next(int n);
	void publish(long sequence);
	long wait_for(long sequence);
private:
	Sequencer *cursor;
	T *entries;
	unsigned int mask;
};

template <typename T>
RingBuffer<T>::RingBuffer(Sequencer *cursor_) : cursor(cursor_) {
	if (cursor->get_size() < 1) {
		throw invalid_argument("buffer_size must not be less than 1");
	}
	if (Util::bitcount(cursor->get_size()) != 1) {
		throw invalid_argument("buffer_size must be a power of 2");
	}
	mask = cursor->get_size() - 1;
	entries = new T[cursor->get_size()]();
}

template <typename T>
T* RingBuffer<T>::get(long sequence) {
	return entries + (sequence & mask);
}

template <typename T>
long RingBuffer<T>::next(int n) {
	if (n < 1) {
		throw invalid_argument("n must be greater than 0");
	}
	return cursor->next(n);
}

template <typename T>
void RingBuffer<T>::publish(long sequence) {
	cursor->publish(sequence);
}

template <typename T>
long RingBuffer<T>::wait_for(long sequence) {
	while (cursor->get() < sequence) {
		usleep(1000);
	}
	return cursor->get();
}
