#pragma once

#include <iostream>
#include <stdint.h>
#include "util.h"

using namespace std;

class Order {
public:
	unsigned char operation;
	unsigned int id;
	unsigned char from;
	unsigned char to;
	short seat;
	int socket_fd;
	int parse(char *raw, int size);
	int dump(char *buffer, int size);
	static bool check(char *raw, int size, int &start, int &end);
};
