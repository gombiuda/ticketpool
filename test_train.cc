#include <iostream>
#include "order.h"
#include "train.h"

#include <assert.h>

using namespace std;

#define SEAT_N 5
#define STATION_N 10

int main() {
	Order *order;
	Train *train;
	// query when empty
	train = new Train(0, STATION_N, SEAT_N);
	order = new Order();
	order->id = 0;
	order->from = 0;
	order->to = STATION_N - 1;
	train->query(order);
	if (order->seat == SEAT_N) {
		cout << "query when empty...ok" << endl;
	} else {
		cout << "query when empty...fail " << order->seat << endl;
	}
	// book one ticket
	train = new Train(0, STATION_N, SEAT_N);
	order = new Order();
	order->id = 1;
	order->from = 0;
	order->to = STATION_N - 1;
	order->seat = -1;
	train->book(order);
	if (order->seat == 0) {
		cout << "book when empty...ok" << endl;
	} else {
		cout << "book when empty...fail " << order->seat << endl;
	}
	// book to full
	cout << "book to full...";
	bool ok = true;
	train = new Train(0, STATION_N, SEAT_N);
	for (int i = 0; i < SEAT_N; i++) {
		order = new Order();
		order->id = i;
		order->from = 0;
		order->to = STATION_N - 1;
		order->seat = -1;
		train->book(order);
		if (order->seat != i) {
			ok = false;
		}
	}
	order = new Order();
	order->id = SEAT_N;
	order->from = 0;
	order->to = STATION_N - 1;
	order->seat = -1;
	train->book(order);
	if (order->seat != -1) {
		ok = false;
	}
	if (ok) {
		cout << "ok" << endl;
	} else {
		cout << "fail" << endl;
	}
	// refund after full
	order = new Order();
	order->id = 0;
	order->from = 0;
	order->to = STATION_N - 1;
	order->seat = 0;
	train->refund(order);
	train->query(order);
	if (order->seat == 1) {
		cout << "refund after full...ok" << endl;
	} else {
		cout << "refund after full...fail" << endl;
	}
	return 0;
}
