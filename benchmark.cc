#include <iostream>
#include <queue>
#include <cstdlib>
#include <time.h>
#include "order.h"
#include "train.h"

using namespace std;

#define N 65536
#define STATION_N 128
#define SEAT_N 2048
#define BILLION 1000000000

// A very simple random number generator.  Not especially good at
// generating truly random bits, but good enough for our needs in this
// package.
class Random {
private:
	uint32_t seed_;
public:
	explicit Random(uint32_t s) : seed_(s & 0x7fffffffu) {
		// Avoid bad seeds.
		if (seed_ == 0 || seed_ == 2147483647L) {
			seed_ = 1;
		}
	}
	uint32_t Next() {
		static const uint32_t M = 2147483647L;   // 2^31-1
		static const uint64_t A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
		// We are computing
		//       seed_ = (seed_ * A) % M,    where M = 2^31-1
		//
		// seed_ must not be zero or M, or else all subsequent computed values
		// will be zero or M respectively.  For all other values, seed_ will end
		// up cycling through every number in [1,M-1]
		uint64_t product = seed_ * A;

		// Compute (product % M) using the fact that ((x << 31) % M) == x.
		seed_ = static_cast<uint32_t>((product >> 31) + (product & M));
		// The first reduction may overflow by 1 bit, so we may need to
		// repeat.  mod == M is not possible; using > allows the faster
		// sign-bit-based test.
		if (seed_ > M) {
			seed_ -= M;
		}
		return seed_;
	}
	// Returns a uniformly distributed value in the range [0..n-1]
	// REQUIRES: n > 0
	uint32_t Uniform(int n) { return Next() % n; }

	// Randomly returns true ~"1/n" of the time, and false otherwise.
	// REQUIRES: n > 0
	bool OneIn(int n) { return (Next() % n) == 0; }

	// Skewed: pick "base" uniformly from range [0,max_log] and then
	// return "base" random bits.  The effect is to pick a number in the
	// range [0,2^max_log-1] with exponential bias towards smaller numbers.
	uint32_t Skewed(int max_log) {
		return Uniform(1 << Uniform(max_log + 1));
	}
};


int main() {
	timespec start, end;
	Train *train = new Train(0, STATION_N, SEAT_N);
	Order *orders = new Order[N]();
	queue<Order*> success_orders;
	for (unsigned int i = 0; i < N; i++) {
		orders[i].id = i;
		orders[i].from = rand() % STATION_N;
		orders[i].to = orders[i].from + rand() % (STATION_N - orders[i].from);
		orders[i].seat = -1;
	}
	Random rnd(301);
	clock_gettime(CLOCK_REALTIME, &start);
	for (unsigned int i = 0; i < N; i++) {
		if (rnd.OneIn(10)) {
			// refund
			if (!success_orders.empty()) {
				train->refund(success_orders.front());
				success_orders.pop();
			}
		} else if (rnd.OneIn(3)) {
			// book
			if (train->book(orders + i) == 0) {
				success_orders.push(orders + i);
			}
		} else {
			// query
			train->query(orders + i);
		}
	}
	clock_gettime(CLOCK_REALTIME, &end);
	double use_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;
	cout << "Process " << N << " requests" << endl;
	cout << "Use time: " << use_time << " s" << endl;
	cout << "Request per second: " << N / use_time << endl;
	return 0;
}
