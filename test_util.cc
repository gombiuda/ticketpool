#include <iostream>
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
	}		
	return 0;
}
