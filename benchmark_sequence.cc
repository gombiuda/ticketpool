#include <iostream>
#include <thread>
#include <sys/time.h>
#include "sequence.h"

using namespace std;

#define C 2
#define N 65536
#define MILLION 1000000

void loop(Sequence *sequence, int n) {
	for (int i = 0; i < n; i++) {
		sequence->set(sequence->get());
	}
}

int main() {
	cout << "size: " << sizeof(Sequence) << endl;
	Sequence *sequences = new Sequence[C]();
	thread threads[C];
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i = 0; i < C; i++) {
		threads[i] = thread(loop, sequences + i, N);
	}
	for (int i = 0; i < C; i++) {
		threads[i].join();
	}
	gettimeofday(&end, NULL);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)MILLION;
	cout << "Process " << C * N << " operations" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "ops: " << dec << (long)(C * N / use_time) << endl;
	return 0;
}
