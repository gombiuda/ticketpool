#include <iostream>
#include <string.h>
#include "bipbuffer.h"

using namespace std;

int main() {
	BipBuffer buffer(12);
	char *index;
	int n;
	index = buffer.reserve(8);
	strcpy(index, "abcdefhi");
	buffer.commit(6);
	n = 6;
	index = buffer.get(n);
	if (index == NULL || strncmp(index, "abcdef", n) != 0) {
		cout << "simple...fail" << endl;
		return -1;
	} else {
		cout << "simple...ok" << endl;
	}
	index = buffer.reserve(3);
	if (index == NULL) {
		cout << "reserve data after commit...fail" << endl;
		return -1;
	} else {
		cout << "reserve data after commit...ok" << endl;
	}
	strcpy(index, "jkl");
	buffer.commit(3);
	if (strncmp(index, "jkl", 3) != 0) {
		cout << "overwrite previous not commited data...fail" << endl;
		return -1;
	} else {
		cout << "overwrite previous not commited data...ok" << endl;
	}
	if (buffer.reserve(13) != NULL) {
		cout << "reserve too many data...fail" << endl;
		return -1;
	} else {
		cout << "reserve too many data...ok" << endl;
	}
	index = buffer.reserve(1);
	if (buffer.reserve(1) != NULL) {
		cout << "reserve when previous reservation not committed...fail" << endl;
		return -1;
	} else {
		cout << "reserve when previous reservation not committed...ok" << endl;
	}
	return 0;
}
