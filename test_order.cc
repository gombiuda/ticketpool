#include <iostream>
#include "order.h"

using namespace std;

int main() {
	char buf_0[20], buf_1[20];
	int size_0, size_1;
	Order order_0, order_1;
	order_0.operation = 0x01;
	order_0.id = 128;
	order_0.from = 1;
	order_0.to = 7;
	order_0.seat = 65;
	if ((size_0 = order_0.dump(buf_0, 20)) == -1) {
		cout << "check dump...fail" << endl;
		exit(1);
	} else {
		cout << "check dump...ok" << endl;
	}
	int start, end;
	if (!Order::check(buf_0, size_0, start, end)) {
		cout << "check dump result...fail" << endl;
		cout << dec << start << " " << end << endl;
		exit(1);
	} else {
		cout << "check dump result...ok" << endl;
	}
	size_1 = order_1.parse(buf_0, size_0);
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
		cout << ((int)order_0.operation & 0xFF) << " " << ((int)order_1.operation & 0xFF) << endl;
		cout << dec << order_0.id << " " << order_1.id << hex << endl;
		cout << ((int)order_0.from & 0xFF) << " " << ((int)order_1.from & 0xFF) << endl;
		cout << ((int)order_0.to & 0xFF) << " " << ((int)order_1.to & 0xFF) << endl;
		cout << order_0.seat << " " << order_1.seat << endl;
		exit(1);
	} else {
		cout << "check parse result...ok" << endl;
	}
	return 0;
}
