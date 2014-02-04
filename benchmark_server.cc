#include <iostream>
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

using namespace std;

#define N 100
#define C 100
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

void send_all(int c, int n, int *fds, int **sizes) {
	char *data = "abcdefghijklmnopqrstuvwxyz";
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < n; j++) {
			if (send(fds[i], data, sizes[i][j], 0) == -1) {
				perror("send");
				exit(1);
			}
		}
	}
}

void recv_all(int epoll_fd, int c, int n, int *fds, int **sizes) {
	int total = 0;
	int BUFFER_SIZE = 1024;
	struct epoll_event events[MAX_EPOLL_EVENTS];
	char buffer[BUFFER_SIZE];
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < n; j++) {
			total += sizes[i][j];
		}
	}
	int ev_n;
	while (total > 0) {
		if ((ev_n = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1)) == -1) {
			cout << "error when epoll wait: " << errno << endl;;
			exit(1);
		}
		int ev_c;
		for (int i = 0; i < ev_n; i++) {
			while (true) {
				if ((ev_c = recv(events[i].data.fd, buffer, BUFFER_SIZE, 0)) == -1) {
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
						break;
					} else {
						cout << "error recv " << events[i].data.fd << endl;
						perror("recv");
						exit(1);
					}
				}
				total -= ev_c;
			}
		}
	}
}

int main() {
	char *host = "127.0.0.1";
	int port = 8000;
	// start server
	thread server_thread([host, port]() {
			Server server(host, port);
			server.start(10);
		});
	server_thread.detach();
	usleep(10000);
	// connect to server
	int fds[C];
	int **sizes;
	char msg[50];
	char buf[20];
	Random rnd(301);
	sizes = new int*[C]();
	for (int i = 0; i < C; i++) {
		sizes[i] = new int[N]();
		for (int j = 0; j < N; j++) {
			sizes[i][j] = rnd.next() % 10 + 10;
		}
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
	thread send_thread(send_all, C, N, fds, sizes);
	thread recv_thread(recv_all, epoll_fd, C, N, fds, sizes);
	send_thread.join();
	recv_thread.join();
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << C * N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << (long)(C * N / use_time) << endl;
	return 0;
}
