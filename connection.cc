#include "connection.h"

#define BUFFER_SIZE 65536
#define CHUNK_SIZE 1024

Connection::Connection(int fd_) : fd(fd_) {
	buffer = new BipBuffer(BUFFER_SIZE);
}

void Connection::handle(RingBuffer<Order> *ring) {
	char *index;
	int n;
	char order[10];
	char msg[50];
	while (true) {
		index = buffer->reserve(CHUNK_SIZE);
		if (index == nullptr) {
			perror("buffer reserve error");
			exit(1);
		}
		if ((n = recv(fd, index, CHUNK_SIZE, 0)) == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				buffer->commit(0);
				break;
			}
		}
		buffer->commit(n);
	}
	index = buffer->get(n);
	while (n > 0) {
		Order *order;
		long sequence;
		int start, end;
		if (Order::check(index, n, start, end)) {
			sequence = ring->next(1);
			order = ring->get(sequence);
			order->parse(index + start, end - start + 1);
			order->socket_fd = fd;
			if (send(fd, index, end, 0) == -1) {
				sprintf(msg, "send error: %d", errno);
				perror(msg);
				exit(1);
			}
			buffer->release(end);
			ring->publish(sequence);
			n -= end;
		} else {
			if (send(fd, index, end, 0) == -1) {
				sprintf(msg, "send error: %d", errno);
				perror(msg);
				exit(1);
			}
			buffer->release(end);
			n -= end;
		}
	}
}

void Connection::close_connection() {
	if (close(fd) == -1) {
		perror("close connection");
		exit(1);
	}
}
