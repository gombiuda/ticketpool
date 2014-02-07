#pragma once

class Sequence {
public:
	Sequence();
	long get();
	void set(long value);
private:
	Sequence(long value);
	static int INITIAL_VALUE;
	long value;
	long padding[7];  // avoid false sharing
};
