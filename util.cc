#include "util.h"

int Util::bitcount(unsigned int v) {
	v = v - ((v >> 1) & 0x55555555);
	v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
	return (((v + (v >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}
