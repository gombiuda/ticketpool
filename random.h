#pragma once

class Random {
private:
	long seed;
public:
	Random(long seed_) : seed(seed_ & 0x7fffffffu) {
		if (seed == 0 || seed == 2147483647L) {
			seed = 1;
		}
	}
	long next() {
		static const long M = 2147483647L;   // 2^31-1
		static const long long A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
		long long product = seed * A;
		seed = static_cast<long>((product >> 31) + (product & M));
		if (seed > M) {
			seed -= M;
		}
		return seed;
	}
	bool OneIn(long n) {
		return (next() % n) == 0;
	}
};
