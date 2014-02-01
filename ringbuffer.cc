#include "ringbuffer.h"

RingBuffer::RingBuffer(unsigned int buffer_size_) {
	if (buffer_size_ < 1) {
		throw invalid_argument("buffer_size must not be less than 1");
	}
	if (Util::bitcount(buffer_size_) != 1) {
		throw invalid_argument("buffer_size must be a power of 2");
	}
	buffer_size = buffer_size_;
	mask = buffer_size - 1;
	cursor = next_cursor = -1;
	entries = new Order[buffer_size]();
}

Order* RingBuffer::get(long sequence) {
	return entries + (sequence & mask);
}

long RingBuffer::next(int n) {
	if (n < 1) {
		throw invalid_argument("n must be greater than 0");
	}
	next_cursor += n;
	return next_cursor;
}

void RingBuffer::publish(long sequence) {
	cursor = sequence;
}

long RingBuffer::wait_for(long sequence) {
	while (cursor < sequence) {}
	return cursor;
}
