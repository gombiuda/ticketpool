#include <iostream>
#include <thread>
#include "ringbuffer.h"
#include "order.h"

using namespace std;

int main() {
	RingBuffer<Order> *ring;
	Order *order;
	long sequence;
	// single next/modify/publish
	ring = new RingBuffer<Order>(16);
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
	ring = new RingBuffer<Order>(4);
	sequence = ring->next(10);
	for (int i = sequence - (10 - 1); i <= sequence; i++) {
		order = ring->get(sequence);
		order->id = i;
		order->from = 0;
		order->to = 9;
		order->seat = -1;
	}
	thread publish([ring]() {
			for (int i = 0; i < 10; i++) {
				ring->publish(i);
			}
		});
	thread consume([ring]() {
			for (int i = 0; i < 10; i++) {
				ring->wait_for(i);
			}
		});
	publish.join();
	consume.join();
	cout << "1-producer/1-consumer...ok" << endl;
	return 0;
}
