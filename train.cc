#include "train.h"
#include <cmath>

Train::Train(unsigned int id_, unsigned char station_number_, unsigned short seat_number_) {
	id = id_;
	station_number = station_number_;
	seat_number = seat_number_;
	orders = new unordered_map<unsigned int, Order>();
	pool = new unsigned char*[seat_number];
	for (int i = 0; i < station_number; i++) {
		pool[i] = new unsigned char[station_number];
	}
}

int Train::query(Order *order) {
	order->seat = 0;
	int length = order->to - order->from + 1;
	for (int i = 0; i < seat_number; i++) {
		if (pool[i][order->from] > length) {
			order->seat++;
		}
	}
	return 0;
}

int Train::book(Order *order) {
	int length = order->to - order->from + 1;
	order->seat = -1;
	// find available seat
	for (int i = 0; i < seat_number; i++) {
		if (pool[i][order->from] > length) {
			order->seat = i;
			break;
		}
	}
	if (order->seat == -1) {
		return -1;
	}
	// update pool
	for (int i = order->to; i >= order->from; i--) {
		pool[order->seat][i] = 0;
	}
	for (int i = order->from - 1; i >=0; i--) {
		if (pool[order->seat][i] == 0) {
			break;
		} else {
			pool[order->seat][i] = order->from - i;
		}
	}
	return 0;
}

int Train::check(Order *order) {
	return 0;
}

int Train::refund(Order *order) {
	for (int i = order->to; i >= order->from; i--) {
		pool[order->seat][i] = order->to - i + 1;
	}
	for (int i = order->from - 1; i >= 0; i--) {
		if (pool[order->seat][i] == 0) {
			break;
		} else {
			pool[order->seat][i] = order->to - i + 1;
		}
	}
	return 0;
}
