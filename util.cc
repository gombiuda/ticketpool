#include "util.h"

int Util::bitcount(unsigned int v) {
	v = v - ((v >> 1) & 0x55555555);
	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
	return (((v + (v >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

int Util::checksum(char *data, int n) {
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum = (sum >> 1) + ((sum & 1) << 7);
		sum += data[i];
		sum &= 0xFF;
	}
	return sum;
}
