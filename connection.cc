#include "connection.h"

#define BUFFER_SIZE 65536
#define CHUNK_SIZE 1024

Connection::Connection(int fd_) : fd(fd_) {
	in = new BipBuffer(BUFFER_SIZE);
	out = new BipBuffer(BUFFER_SIZE);
}

void Connection::handle(RingBuffer<Order> *ring, Train *train) {
	char *index_in, *index_out;
	int n;
	Order *order;
	long sequence = -1;
	int start, end;
	index_in = in->reserve(CHUNK_SIZE);
	if (index_in == nullptr) {
		perror("in reserve error");
		exit(1);
	}
	if ((n = recv(fd, index_in, CHUNK_SIZE, 0)) == -1) {
		perror("connection recv");
		exit(1);
	}
	in->commit(n);
	index_in = in->get(n);
	// test-start
	/* Order::check(index_in, n, start, end); */
	/* sequence = ring->next(1); */
	/* order = ring->get(sequence); */
	/* order->parse(index_in + start, end - start + 1); */
	/* if (send(fd, index_in, n, 0) == -1) { */
	/* 	perror("connection send"); */
	/* 	exit(1); */
	/* } */
	/* in->release(n); */
	/* return; */
	/* // test-end */
	int count = 0;
	while (n >= 12) {
		if (Order::check(index_in, n, start, end)) {
			sequence = ring->next(1);
			order = ring->get(sequence);
			if (order->parse(index_in + start, end - start + 1) == -1) {
				cout << "order parse fail" << endl;
				exit(1);
			}
			order->connection = this;
			/* // test-start */
			/* switch (order->operation) { */
			/* case 0x01: */
			/* 	train->query(order); */
			/* 	break; */
			/* case 0x02: */
			/* 	train->book(order); */
			/* 	break; */
			/* case 0x03: */
			/* 	train->check(order); */
			/* 	break; */
			/* case 0x04: */
			/* 	train->refund(order); */
			/* 	break; */
			/* default: */
			/* 	cout << "unknown operation " << hex << order->operation << endl; */
			/* 	exit(1); */
			/* } */
			/* if (order->dump() == -1) { */
			/* 	cout << "order dump fail" << endl; */
			/* 	exit(1); */
			/* } */
			/* index_out = out->reserve(order->rsize); */
			/* if (index_out == nullptr) { */
			/* 	perror("in reserve error"); */
			/* 	exit(1); */
			/* } */
			/* memcpy(index_out, order->raw, order->rsize); */
			/* out->commit(order->rsize); */
			/* // test-end */
			in->release(end);
			count++;
		} else {
			if (end != 0) {
				cout << dec << start << "-" << end << " bytes unknown" << endl;
				Util::print_array(index_in, end);
			}
			in->release(end);
			if (end == 0) {
				// not enough data, wait
				break;
			}
		}
		n -= end;
		index_in += end;
	}
	/* // test-start */
	/* /\* cout << dec << count << " orders got" << endl; *\/ */
	/* index_out = out->get(n); */
	/* while (n > 0) { */
	/* 	if (send(fd, index_out, n, 0) == -1) { */
	/* 		perror("connection send"); */
	/* 		exit(1); */
	/* 	} */
	/* 	out->release(n); */
	/* 	index_out = out->get(n); */
	/* } */
	/* // test-end */
	if (sequence != -1) {
		ring->publish(sequence);
	}
}

void Connection::flush() {
	char *index;
	int n;
	while (true) {
		index = out->get(n);
		if (n > 0) {
			// TODO: should handle EAGAIN
			if (send(fd, index, n, 0) == -1) {
				perror("connection send");
				exit(1);
			}
			out->release(n);
		} else {
			break;
		}
	}
}

void Connection::close_connection() {
	if (close(fd) == -1) {
		perror("close connection");
		exit(1);
	}
}
