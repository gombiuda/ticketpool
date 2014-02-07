#include <iostream>
#include <unordered_map>
#include <unistd.h>
#include <thread>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "server.h"
#include "random.h"
#include "util.h"

using namespace std;

#define N 200
#define C 50
#define MILLION 1000000

int monitor_all(int c, int *fds) {
	int epoll_fd;
	struct epoll_event ev;
	if ((epoll_fd = epoll_create(1)) == -1) {
		cout << "error when create epoll: " << errno << endl;;
		exit(1);
	}
	for (int i = 0; i < c; i++) {
		if (fcntl(fds[i], F_SETFL, fcntl(fds[i], F_GETFD, 0)|O_NONBLOCK) == -1) {
			cout << "error when set nonblocking: " << errno << endl;;
			exit(1);
		}
		ev.events = EPOLLIN | EPOLLET;
		ev.data.fd = fds[i];
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fds[i], &ev) == -1) {
			cout << "error when add fd to epoll: " << errno << endl;;
			exit(1);
		}
	}
	return epoll_fd;
}

void send_all(int c, int n, int *fds) {
	Order order;
	Random rnd(301);
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < n; j++) {
			if (rnd.OneIn(3)) {
				order.operation = 0x02;
			} else {
				order.operation = 0x01;
			}
			order.id = (i << 8) + j;
			order.from = rnd.next() % 128;
			order.to = rnd.next() % 128 + order.from;
			order.seat = -1;
			order.dump();
			if (send(fds[i], order.raw, order.rsize, 0) == -1) {
				perror("send");
				exit(1);
			}
		}
	}
}

void recv_all(int epoll_fd, int c, int n, int *fds) {
	unordered_map<int, BipBuffer*> buffers;
	for (int i = 0; i < c; i++) {
		buffers.insert(make_pair(fds[i], new BipBuffer(10240)));
	}
	struct epoll_event events[MAX_EPOLL_EVENTS];
	int ev_n;
	int total = c * n;
	Order *order = new Order();
	int CHUNK_SIZE = 1024;
	char *index;
	while (total > 0) {
		if ((ev_n = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1)) == -1) {
			perror("epoll wait");
			exit(1);
		}
		for (int i = 0; i < ev_n; i++) {
			BipBuffer *buffer = buffers[events[i].data.fd];
			int size = 0;
			while (true) {
				index = buffer->reserve(CHUNK_SIZE);
				if ((size = recv(events[i].data.fd, index, CHUNK_SIZE, 0)) == -1) {
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
						buffer->commit(0);
						break;
					} else {
						perror("recv");
						exit(1);
					}
				}
				buffer->commit(size);
			}
			index = buffer->get(size);
			while (size > 0) {
				int start, end;
				if (Order::check(index, size, start, end)) {
					order->rsize = end - start + 1;
					memcpy(order->raw, index + start, order->rsize);
					if (order->parse() == -1) {
						cout << "order parse fail" << endl;
						exit(1);
					}
					buffer->release(end);
					total--;
					/* cout << dec << total << " orders remain" << endl; */
				} else {
					if (end != 0) {
						cout << dec << end << " bytes unknown" << endl;
						Util::print_array(index, end);
					}
					buffer->release(end);
					if (end == 0) {
						break;
					}
				}
				size -= end;
				index += end;
			}
		}
	}
}

int main() {
	char *host = "127.0.0.1";
	int port = 8000;
	// start server
	Server server(host, port);
	server.start(10);
	usleep(10000);
	// connect to server
	int fds[C];
	for (int i = 0; i < C; i++) {
		if ((fds[i] = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("socket");
			exit(1);
		}
		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(host);
		address.sin_port = htons(port);
		if (connect(fds[i], (struct sockaddr*)&address, sizeof(address)) == -1) {
			perror("connect");
			exit(1);
		}
	}
	int epoll_fd = monitor_all(C, fds);
	struct timeval start, end;
	gettimeofday(&start, NULL);
	thread send_thread(send_all, C, N, fds);
	thread recv_thread(recv_all, epoll_fd, C, N, fds);
	send_thread.join();
	recv_thread.join();
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << C * N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << dec << (long)(C * N / use_time) << endl;
	return 0;
}
