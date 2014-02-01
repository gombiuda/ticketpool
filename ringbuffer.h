#pragma once

#include <stdexcept>
#include "order.h"
#include "util.h"

using namespace std;

class RingBuffer {
public:
	RingBuffer(unsigned int buffer_size);
	Order* get(long sequence);
	long next(int n);
	void publish(long sequence);
	long wait_for(long sequence);
private:
	unsigned int buffer_size;
	long cursor, next_cursor;
	Order *entries;
	unsigned int mask;
};
