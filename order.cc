#include "order.h"

// return bytes used
bool Order::check(char *raw, int size, int &start, int &end) {
	if (size < 12) {
		// the packet size is no less than 12 bytes, including delimiter
		end = 0;
		return false;
	}
	start = 0;
	end = 0;
	while (start < size && (uint8_t)raw[start]!= 0xF6) {
		// find start delimiter
		start++;
	}
	if (start >= size || (uint8_t)raw[start] != 0xF6) {
		// start delimiter not found, drop data
		end = size;
		cout << "start delimiter not found, drop data" << endl;
		return false;
	}
	while ((uint8_t)raw[start] == 0xF6) {
		// drop delimiter
		start++;
	}
	end = start + 1;
	while (end < size && (uint8_t)raw[end] != 0xF6) {
		// find end delimiter
		end++;
	}
	if (end >= size || (uint8_t)raw[end] != 0xF6) {
		// end delimiter not found, give up parsing
		end = 0;
		cout << "end delimiter not found, give up parsing" << endl;
		return false;
	}
	int count = 0;
	for (int i = start; i < end; i++) {
		if ((uint8_t)raw[i] == 0xF7) {
			// count escaper
			count++;
		}
	}
	if (end - start - count != 10) {
		// packet length not valid, drop data
		cout << "packet length not valid, drop data" << endl;
		return false;
	}
	int sum = Util::checksum(raw + start, end - start - 1);
	if (sum != raw[end - 1]) {
		// last byte is the checksum, even if it's escaped
		// checksum invalid, drop data
		end--;
		cout << "checksum invalid, drop data" << endl;
		return false;
	}
	end--;
	return true;
}

// return bytes used
int Order::parse(char *raw, int size) {
	int index = 0;
	while ((uint8_t)raw[index] == 0xF6) {
		index++;
	}
	int psize;
	if ((psize = Util::parse<unsigned char>(raw + index, size - index, operation, 1)) == -1) {
		cout << "parse operation fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned int>(raw + index, size - index, id, 4)) == -1) {
		cout << "parse id fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned char>(raw + index, size - index, from, 1)) == -1) {
		cout << "parse from fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned char>(raw + index, size - index, to, 1)) == -1) {
		cout << "parse to fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<short>(raw + index, size - index, seat, 2)) == -1) {
		cout << "parse seat fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	return index;
}

int Order::dump(char *buffer, int size) {
	if (size < 10) {
		// not enough space
		return -1;
	}
	int index = 0;
	buffer[index++] = 0xF6;
	int dsize;
	if ((dsize = Util::dump<unsigned char>(buffer + index, size - index, operation, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned int>(buffer + index, size - index, id, 4)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned char>(buffer + index, size - index, from, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned char>(buffer + index, size - index, to, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<short>(buffer + index, size - index, seat, 2)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	int sum = Util::checksum(buffer + 1, index - 1);
	if ((dsize = Util::dump<unsigned char>(buffer + index, size - index, sum, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	buffer[index++] = 0xF6;
	return index;
}
