#include <iostream>
#include <sys/time.h>
#include "random.h"
#include "order.h"

#define N 65536
#define MILLION 1000000

int main() {
	cout << "size: " << sizeof(Order) << endl;
	Random rnd(301);
	Order order;
	order.operation = 0x01;
	order.id = rnd.next();
	order.from = rnd.next() % 256;
	order.to = rnd.next() % 256 + order.from;
	order.seat = rnd.next() % 128;
	struct timeval start, end;
	double use_time;
	// benchmark dump
	gettimeofday(&start, NULL);
	for (int i = 0; i < N; i++) {
		order.dump();
	}
	gettimeofday(&end, NULL);
	use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " dump" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << dec << (long)(N / use_time) << endl;
	// benchmark check
	gettimeofday(&start, NULL);
	int s, e;
	for (int i = 0; i < N; i++) {
		Order::check(order.raw, order.rsize, s, e);
	}
	gettimeofday(&end, NULL);
	use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " check" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << dec << (long)(N / use_time) << endl;
	// benchmark parse
	gettimeofday(&start, NULL);
	for (int i = 0; i < N; i++) {
		order.parse();
	}
	gettimeofday(&end, NULL);
	use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " parse" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << dec << (long)(N / use_time) << endl;
	return 0;
}
