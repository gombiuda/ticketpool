#include <iostream>
#include <thread>
#include <cstdlib>
#include <sys/time.h>
#include "random.h"
#include "order.h"
#include "ringbuffer.h"
#include "sequencer.h"

using namespace std;

static long N = 1 << 24;

#define STATION_N 128
#define SEAT_N 2048
#define MILLION 1000000

void produce(RingBuffer<Order> *ring, int n) {
	long sequence;
	Random rnd(301);
	Order *order;
	for (int i = 0; i < n; i++) {
		sequence = ring->next(1);
		order = ring->get(sequence);
		order->id = i;
		order->from = rnd.next() % STATION_N;
		order->to = order->from + rnd.next() % (STATION_N - order->from);
		order->seat = -1;
		ring->publish(i);
	}
}

void consume(RingBuffer<Order> *ring, int n) {
	long sequence = -1, count = 0;
	Random rnd(301);
	Order *order;
	while (count < n) {
		sequence++;
		long hi = ring->wait_for(sequence);
		long lo = sequence;
		for (int i = lo; i <= hi; i++) {
			order = ring->get(i);
			order->seat = rnd.next() % SEAT_N;
		}
		count += hi - lo + 1;
		sequence = hi;
	}
}

int main() {
	struct timeval start, end;
	Sequencer *sequencer = new Sequencer(65536);
	RingBuffer<Order> *ring = new RingBuffer<Order>(sequencer);
	gettimeofday(&start, NULL);
	thread producer(produce, ring, N);
	thread consumer(consume, ring, N);
	producer.join();
	consumer.join();
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << (long)(N / use_time) << endl;
	return 0;
}
