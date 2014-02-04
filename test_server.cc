#include <iostream>
#include <unistd.h>
#include <thread>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"

using namespace std;

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
	int connect_fd;
	char msg[50];
	char buf[100];
	char *data = "abcdefghijklmnopqrstuvwxyz";
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
	if (send(connect_fd, data, 6, 0) == -1) {
		perror("send");
		exit(1);
	}
	bzero(buf, sizeof(buf));
	if (recv(connect_fd, buf, 6, 0) == -1) {
		perror("recv");
		exit(1);
	}
	if (strncmp(buf, data, 6) != 0) {
		cout << "match...fail" << endl;
		exit(1);
	} else {
		cout << "match...ok" << endl;
	}
	
	return 0;
}
