#pragma once

class BipBuffer {
public:
	BipBuffer(int size);
	char* reserve(int n);
	void commit(int n);
	char* get(int &n);
	void release(int n);
private:
	char *buffer;
	int size;
	int size_a, size_b, size_r;
	int index_a, index_b, index_r;
};
