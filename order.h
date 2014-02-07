#pragma once

#include <iostream>
#include <stdint.h>
#include "util.h"

using namespace std;

#define RAW_SIZE 20

class Order {
public:
	unsigned char operation;
	unsigned int id;
	unsigned char from;
	unsigned char to;
	short seat;
	char raw[RAW_SIZE];
	int rsize;
	int socket_fd;
	int parse();
	int dump();
	static bool check(char *raw, int size, int &start, int &end);
};
