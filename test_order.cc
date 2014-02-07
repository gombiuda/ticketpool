#include <iostream>
#include <string.h>
#include "order.h"
#include "util.h"

using namespace std;

int main() {
	int size_0, size_1;
	Order order_0, order_1;
	order_0.operation = 0x01;
	order_0.id = 128;
	order_0.from = 1;
	order_0.to = 7;
	order_0.seat = 65;
	if ((size_0 = order_0.dump()) == -1) {
		cout << "check dump...fail" << endl;
		exit(1);
	} else {
		cout << "check dump...ok" << endl;
	}
	int start, end;
	if (!Order::check(order_0.raw, order_0.rsize, start, end)) {
		cout << "check dump result...fail" << endl;
		cout << dec << start << " " << end << endl;
		exit(1);
	} else {
		cout << "check dump result...ok" << endl;
	}
	memcpy(order_1.raw, order_0.raw, order_0.rsize);
	order_1.rsize = order_0.rsize;
	size_1 = order_1.parse();
	if (size_0 - 2 != size_1) {  // include 2 delimiter
		cout << "check parse size...fail" << endl;
		cout << dec << size_0 << " " << size_1 << endl;
		exit(1);
	} else {
		cout << "check parse size...ok" << endl;
	}
	if (order_0.operation != order_1.operation || \
	    order_0.id != order_1.id || \
	    order_0.from != order_1.from || \
	    order_0.to != order_1.to || \
	    order_0.seat != order_1.seat) {
		cout << "check parse result...fail" << endl;
		exit(1);
	} else {
		cout << "check parse result...ok" << endl;
	}
	return 0;
}
