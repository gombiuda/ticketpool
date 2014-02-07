#include "connection.h"

#define BUFFER_SIZE 65536
#define CHUNK_SIZE 1024

Connection::Connection(int fd_) : fd(fd_) {
	buffer = new BipBuffer(BUFFER_SIZE);
}

void Connection::handle(RingBuffer<Order> *ring) {
	char *index;
	int n;
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
			} else {
				perror("connection recv");
				exit(1);
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
			order->rsize = end - start + 1;
			memcpy(order->raw, index + start, order->rsize);
			if (order->parse() == -1) {
				cout << "order parse fail" << endl;
				exit(1);
			}
			order->socket_fd = fd;
			buffer->release(end);
			ring->publish(sequence);
		} else {
			if (end != 0) {
				cout << dec << start << "-" << end << " bytes unknown" << endl;
				Util::print_array(index, end);
			}
			buffer->release(end);
			if (end == 0) {
				// not enough data, wait
				break;
			}
		}
		n -= end;
		index += end;
	}
}

void Connection::close_connection() {
	if (close(fd) == -1) {
		perror("close connection");
		exit(1);
	}
}
