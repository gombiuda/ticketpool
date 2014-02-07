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
};
