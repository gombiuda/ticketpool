#include "server.h"

Server::Server(const char* host_, int port_) : host(host_), port(port_) {
	ring = new RingBuffer<Order>(1024);
}

void Server::start(int backlog) {
	struct sockaddr_in address;
	struct epoll_event ev;
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		cout << "error when create socket: " << errno << endl;
		exit(1);
	}
	memset(&address, 0, sizeof(address));
	if (inet_addr(host) == -1) {
		cout << "error when converting host: " << host << endl;
		exit(1);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(host);
	address.sin_port = htons(port);
	if (bind(listen_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		cout << "error when bind socket: " << errno << endl;;
		exit(1);
	}
	if (listen(listen_fd, backlog) == -1) {
		cout << "error when listen socket: " << errno << endl;;
		exit(1);
	}
	if ((epoll_fd = epoll_create(1)) == -1) {
		cout << "error when create epoll: " << errno << endl;;
		exit(1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
		cout << "error when add listen_fd to epoll: " << errno << endl;;
		exit(1);
	}
	handle_events();
}

void Server::handle_events() {
	while (!stopped) {
		int n;
		if ((n = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1)) == -1) {
			cout << "error when epoll wait: " << errno << endl;;
			exit(1);
		}
		for (int i = 0; i < n; i++) {
			if (events[i].data.fd == listen_fd) {
				accept_connection();
			} else if (events[i].events & EPOLLRDHUP) {
				close_connection(events[i].data.fd);
			} else if (events[i].events & EPOLLIN) {
				receive_data(events[i].data.fd);
			}
		}
	}
}

void Server::accept_connection() {
	struct epoll_event ev;
	if ((connect_fd = accept(listen_fd, NULL, NULL)) == -1) {
		cout << "error when accept: " << errno << endl;;
		exit(1);
	}
	if (fcntl(connect_fd, F_SETFL, fcntl(connect_fd, F_GETFD, 0)|O_NONBLOCK) == -1) {
		cout << "error when set nonblocking: " << errno << endl;;
		exit(1);
	}
	connections.insert(make_pair(connect_fd, new Connection(connect_fd)));
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
	ev.data.fd = connect_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connect_fd, &ev) == -1) {
		connections.erase(connect_fd);
		cout << "error when add connect fd to epoll: " << errno << endl;;
		exit(1);
	}
}

void Server::close_connection(int fd) {
	unordered_map<int, Connection*>::const_iterator pair = connections.find(fd);
	if (pair == connections.end()) {
		cout << "connection not found" << endl;
		exit(1);
	}
	Connection *connection = pair->second;
	connection->close_connection();
	connections.erase(fd);
}

void Server::receive_data(int fd) {
	unordered_map<int, Connection*>::const_iterator pair = connections.find(fd);
	if (pair == connections.end()) {
		cout << "connection not found" << endl;
		exit(1);
	}
	Connection *connection = pair->second;
	connection->handle(ring);
}
