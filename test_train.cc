#include <iostream>
#include <queue>
#include <cstdlib>
#include <time.h>
#include "order.h"
#include "train.h"

using namespace std;

int main() {
	Order *order;
	Train train(0, 10, 5);
	// query when empty
	order = new Order();
	order->id = 0;
	order->from = 0;
	order->to = 9;
	train.query(order);
	assert order->seat == 5;
	// book one ticket
	order = new Order();
	order->id = 1;
	order->from = 0;
	order->to = 9;
	train.book(order);
	assert order->seat == 0;
	return 0;
}
