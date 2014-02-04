#include <iostream>
#include <string.h>
#include <stdio.h>
#include "util.h"

using namespace std;

int main() {
	if (Util::bitcount(0) == 0 && \
	    Util::bitcount(1) == 1 && \
	    Util::bitcount(2) == 1 && \
	    Util::bitcount(3) == 2 && \
	    Util::bitcount(4) == 1 && \
	    Util::bitcount(5) == 2 && \
	    Util::bitcount(6) == 2 && \
	    Util::bitcount(7) == 3 && \
	    Util::bitcount(8) == 1) {
		cout << "bitcount...ok" << endl;
	} else {
		cout << "bitcount...fail" << endl;
		exit(1);
	}
	char buffer[10];
	unsigned int data = 129;
	if (Util::dump<unsigned int>(buffer, 10, data, 4) == -1) {
		cout << "dump...fail" << endl;
		exit(1);
	}
	if (data != (((uint8_t)(buffer[0]) << 24) + \
		     ((uint8_t)(buffer[1]) << 16) + \
		     ((uint8_t)(buffer[2]) << 8) + \
		     (uint8_t)buffer[3])) {
		cout << "check dump...fail" << endl;
		exit(1);
	} else {
		cout << "check dump...ok" << endl;
	}
	unsigned int result = 0;
	if (Util::parse<unsigned int>(buffer, 10, result, 4) == -1)  {
		cout << "parse...fail" << endl;
		exit(1);
	}
	if (data != result) {
		cout << "check parse...fail" << endl;
		exit(1);
	} else {
		cout << "check parse...ok" << endl;
	}
	return 0;
}
