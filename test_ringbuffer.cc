#include <iostream>
#include "ringbuffer.h"
#include "order.h"

using namespace std;

int main() {
	RingBuffer *ring;
	Order *order;
	long sequence;
	// single next/modify/publish
	ring = new RingBuffer(16);
	sequence = ring->next(1);
	order = ring->get(sequence);
	order->id = 0;
	order->from = 0;
	order->to = 9;
	order->seat = -1;
	ring->publish(sequence);
	if (sequence == ring->wait_for(sequence)) {
		cout << "single next/get/publish...ok" << endl;
	} else {
		cout << "single next/get/publish...fail" << endl;
	}
	// multiple next/modify/publish
	ring = new RingBuffer(16);
	sequence = ring->next(3);
	for (int i = sequence - 2; i <= sequence; i++) {
		order = ring->get(sequence);
		order->id = i;
		order->from = 0;
		order->to = 9;
		order->seat = -1;
	}
	ring->publish(2);
	if (2 == ring->wait_for(1)) {
		cout << "multiple next/get/publish...ok" << endl;
	} else {
		cout << "multiple next/get/publish...fail" << endl;
	}
	return 0;
}
