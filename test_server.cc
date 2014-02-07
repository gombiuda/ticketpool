#include <iostream>
#include <unistd.h>
#include <thread>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "server.h"
#include "util.h"

using namespace std;

int main() {
	char *host = "127.0.0.1";
	int port = 8000;
	// start server
	Server server(host, port);
	thread server_thread([](Server *server) {
			server->start(10);
		}, &server);
	server_thread.detach();
	usleep(10000);
	cout << "server start...ok" << endl;
	// connect to server
	int connect_fd;
	if ((connect_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(host);
	address.sin_port = htons(port);
	if (connect(connect_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		perror("connect");
		exit(1);
	}
	cout << "connect...ok" << endl;
	if (fcntl(connect_fd, F_SETFL, fcntl(connect_fd, F_GETFD, 0)|O_NONBLOCK) == -1) {
		cout << "error when set nonblocking: " << errno << endl;;
		exit(1);
	}
	Order *order_0 = new Order();
	order_0->operation = 0x02;
	order_0->id = 1;
	order_0->from = 1;
	order_0->to = 14;
	order_0->seat = -1;
	order_0->dump();
	if (send(connect_fd, order_0->raw, order_0->rsize, 0) == -1) {
		perror("send");
		exit(1);
	}
	cout << "order send...ok" << endl;
	Order *order_1 = new Order();
	int n = 0;
	while (n < 10) {
		while (true) {
			int c;
			if ((c = recv(connect_fd, order_1->raw + n, RAW_SIZE - n, 0)) == -1) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					break;
				} else {
					perror("recv");
					exit(1);
				}
			}
			n += c;
		}
	}
	cout << "wait response...ok" << endl;
	order_1->rsize = n;
	int start, end;
	if (!Order::check(order_1->raw, order_1->rsize, start, end)) {
		cout << "check response...fail" << endl;
		exit(1);
	}
	if (order_1->parse() == -1) {
		cout << "parse response...fail" << endl;
		exit(1);
	}
	cout << "parse as order...ok" << endl;
	if (order_0->operation != order_1->operation || \
	    order_0->id != order_1->id || \
	    order_0->from != order_1->from || \
	    order_0->to != order_1->to) {
		cout << "order match...fail" << endl;
		exit(1);
	} else {
		cout << "order match...ok" << endl;
	}
	return 0;
}
