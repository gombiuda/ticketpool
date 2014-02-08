#pragma once

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "bipbuffer.h"
#include "ringbuffer.h"
#include "order.h"
#include "train.h"

using namespace std;

class Connection {
public:
	Connection(int fd);
	BipBuffer *in, *out;
	void handle(RingBuffer<Order> *ring, Train *train);
	void flush();
	void close_connection();
private:
	int fd;
	double use_time;
};
