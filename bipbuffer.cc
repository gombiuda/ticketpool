#include "bipbuffer.h"

BipBuffer::BipBuffer(int size_) : size(size_) {
	size_a = size_b = size_r = index_a = index_b = index_r = 0;
	buffer = new char[size]();
}

char* BipBuffer::reserve(int n) {
	if (size_r != 0) {
		// previous reservation is not committed
		return nullptr;
	}
	int a_free = size - (index_a + size_a);
	int b_free = index_a - (index_b + size_b);
	if (size_b) {
		if (n > b_free) {
			return nullptr;
		}
		index_r = index_b + size_b;
		size_r = n;
	} else {
		if (a_free >= b_free) {
			if (n > a_free) {
				return nullptr;
			}
			index_r = index_a + size_a;
			size_r = n;
		} else {
			if (n > b_free) {
				return nullptr;
			}
			index_r = index_b + size_b;
			size_r = n;
		}
	}
	return buffer + index_r;
}

void BipBuffer::commit(int n) {
	if (n == 0) {
		index_r = size_r = 0;
		return;
	}
	if (n > size_r) {
		// just commit the size reserved before
		n = size_r;
	}
	if (size_a == 0 && size_b == 0) {
		index_a = index_r;
		size_a = n;
		index_r = size_r = 0;
		return;
	}
	if (index_r == index_a + size_a) {
		size_a += n;
	} else {
		size_b += n;
	}
	index_r = size_r = 0;
}

char* BipBuffer::get(int &n) {
	if (size_a == 0) {
		n = 0;
		return nullptr;
	}
	n = size_a;
	return buffer + index_a;
}

void BipBuffer::release(int n) {
	if (n >= size_a) {
		index_a = index_b;
		size_a = size_b;
		index_b = size_b = 0;
	} else {
		size_a -= n;
		index_a += n;
	}
}
