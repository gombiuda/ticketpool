#pragma once

#include <vector>
#include <unordered_map>
#include "order.h"

using namespace std;

class Train {
public:
	Train(unsigned int id, unsigned char station_number, unsigned short seat_number);
	int query(Order *order);
	int book(Order *order);
	int check(Order *order);
	int refund(Order *order);
private:
	unsigned int id;
	unsigned char station_number;
	unsigned short seat_number;
	unordered_map<unsigned int, Order> *orders;
	unsigned char **pool;
};
