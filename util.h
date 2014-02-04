#pragma once

#include <stdint.h>

class Util {
public:
	static int bitcount(unsigned int v);
	static int checksum(char *data, int n);
	template<typename T> static int dump(char *buffer, int buf_size, T data, int n);
	template<typename T> static int parse(char *buffer, int buf_size, T &data, int n);
};

template<typename T> int Util::dump(char *buffer, int buf_size, T data, int n) {
	int index = 0;
	n -= 1;
	while (n >= 0) {
		if ((data >> (n * 8)) & 0xFF == 0xF6) {
			if (index > buf_size) {
				return -1;
			}
			buffer[index++] = 0xF7;
		}
		if (index > buf_size) {
			return -1;
		}
		buffer[index++] = (data >> (n * 8)) & 0xFF;
		n--;
	}
	return index;
}

template<typename T> int Util::parse(char *buffer, int buf_size, T &data, int n) {
	data = 0;
	int index = 0;
	while (n > 0) {
		if (index >= buf_size) {
			return -1;
		} else if ((uint8_t)buffer[index] == 0xF7) {
			index++;
		} else {
			data = (data << 8) + (uint8_t)buffer[index];
			index++;
			n--;
		}
	}
	return index;
}
