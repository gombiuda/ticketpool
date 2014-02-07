#include <iostream>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include "order.h"
#include "train.h"
#include "random.h"

using namespace std;

#define N 65536
#define STATION_N 64
#define SEAT_N 512
#define MILLION 1000000

int main() {
	struct timeval start, end;
	Train *train = new Train(0, STATION_N, SEAT_N);
	Order *orders = new Order[N]();
	queue<Order*> success_orders;
	for (unsigned int i = 0; i < N; i++) {
		orders[i].id = i;
		orders[i].from = rand() % STATION_N;
		orders[i].to = orders[i].from + rand() % (STATION_N - orders[i].from);
		orders[i].seat = -1;
	}
	Random rnd(301);
	gettimeofday(&start, NULL);
	for (unsigned int i = 0; i < N; i++) {
		if (rnd.OneIn(10)) {
			// refund
			if (!success_orders.empty()) {
				train->refund(success_orders.front());
				success_orders.pop();
			}
		} else if (rnd.OneIn(3)) {
			// book
			if (train->book(orders + i) == 0) {
				success_orders.push(orders + i);
			}
		} else {
			// query
			train->query(orders + i);
		}
	}
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << (long)(N / use_time) << endl;
	return 0;
}
