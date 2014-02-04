#pragma once

#include <stdexcept>
#include <unistd.h>
#include "util.h"

using namespace std;

template <typename T>
class RingBuffer {
public:
	RingBuffer(unsigned int buffer_size);
	T* get(long sequence);
	long next(int n);
	void publish(long sequence);
	long wait_for(long sequence);
private:
	unsigned int buffer_size;
	long cursor, next_cursor;
	T *entries;
	unsigned int mask;
};

template <typename T>
RingBuffer<T>::RingBuffer(unsigned int buffer_size_) {
	if (buffer_size_ < 1) {
		throw invalid_argument("buffer_size must not be less than 1");
	}
	if (Util::bitcount(buffer_size_) != 1) {
		throw invalid_argument("buffer_size must be a power of 2");
	}
	buffer_size = buffer_size_;
	mask = buffer_size - 1;
	cursor = next_cursor = -1;
	entries = new T[buffer_size]();
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
	next_cursor += n;
	return next_cursor;
}

template <typename T>
void RingBuffer<T>::publish(long sequence) {
	cursor = sequence;
}

template <typename T>
long RingBuffer<T>::wait_for(long sequence) {
	while (cursor < sequence) {
		usleep(1000);
	}
	return cursor;
}
