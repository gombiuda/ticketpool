#pragma once

#include <iostream>
#include <stdexcept>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unordered_map>
#include <thread>
#include <omp.h>
#include "connection.h"
#include "ringbuffer.h"
#include "sequencer.h"
#include "order.h"
#include "train.h"

using namespace std;

#define MAX_EPOLL_EVENTS 10

class Server {
public:
	Server(const char* host, int port);
	void start(int backlog);
	void handle_orders();
private:
	int listen_fd, epoll_fd, connect_fd;
	const char* host;
	int port;
	struct epoll_event events[MAX_EPOLL_EVENTS];
	unordered_map<int, Connection*> connections;
	RingBuffer<Order> *ring;
	Sequencer *sequencer;
	long sequence;
	Train *train;
	thread *connection_handler, *order_handler;
	void accept_connection();
	void close_connection(int fd);
	void receive_data(int fd);
	void handle_connections();
};
