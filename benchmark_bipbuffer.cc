#include <iostream>
#include <string.h>
#include <sys/time.h>
#include "random.h"
#include "bipbuffer.h"

using namespace std;

#define N 65535
#define MILLION 1000000

int main() {
	const char *data = "abcdefghijklmnopqrstuvwxyz";
	struct timeval start, end;
	BipBuffer buffer(1024);
	char *index;
	int n, m;
	Random rnd(301);
	gettimeofday(&start, NULL);
	for (int i = 0; i < N; i++) {
		n = rnd.next() % 26;
		index = buffer.reserve(n);
		strncpy(index, data, n);
		buffer.commit(n);
		m = n;
		index = buffer.get(m);
		if (strncmp(index, data, m) != 0) {
			cout << "data not matched" << endl;
			exit(1);
		}
		buffer.release(m);
	}
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << (long)(N / use_time) << endl;
	return 0;
}
