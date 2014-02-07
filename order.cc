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
	while (start < size && (uint8_t)raw[start] != 0xF6) {
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
	start--;
	end = start + 1;
	int count = 10;  // packet length
	bool escaped = false;
	int checksum_index, checksum;
	// find end delimiter
	while (end < size) {
		if ((uint8_t)raw[end] == 0xF7 && !escaped) {
			escaped = true;
		} else {
			if (!escaped && (uint8_t)raw[end] == 0xF6) {
				break;
			}
			if (count == 1) {
				if (escaped) {
					checksum_index = end - 1;
				} else {
					checksum_index = end;
				}
				checksum = (uint8_t)raw[end];
			}
			if (escaped) {
				escaped = false;
			}
			count--;
		}
		end++;
	}
	if (end >= size) {
		if (count >= 0) {
			// not enought data, do no drop them
			end = 0;
		} else {
			cout << "end delimiter not found, drop data " << dec << end << endl;
		}
		return false;
	} else if (count != 0) {
		// invalid end delimiter
		Util::print_array(raw + start, end - start + 1);
		end--;  // keep delimiter
		cout << "invalid end delimiter, drop data " << dec << end << endl;
		return false;
	}
	// end points to end delimiter
	if (Util::checksum(raw + start + 1, checksum_index - (start + 1)) != checksum) {
		// last byte is the checksum, even if it's escaped
		// checksum invalid, drop data
		cout << "checksum invalid, drop data " << hex << checksum << endl;
		Util::print_array(raw + start + 1, checksum_index - (start + 1) + 1);
		end--;  // keep end delimiter
		return false;
	}
	return true;
}

// return bytes used
int Order::parse() {
	int index = 0;
	while ((uint8_t)raw[index] == 0xF6) {
		index++;
	}
	int psize;
	if ((psize = Util::parse<unsigned char>(raw + index, RAW_SIZE - index, operation, 1)) == -1) {
		cout << "parse operation fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned int>(raw + index, RAW_SIZE - index, id, 4)) == -1) {
		cout << "parse id fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned char>(raw + index, RAW_SIZE - index, from, 1)) == -1) {
		cout << "parse from fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<unsigned char>(raw + index, RAW_SIZE - index, to, 1)) == -1) {
		cout << "parse to fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	if ((psize = Util::parse<short>(raw + index, RAW_SIZE - index, seat, 2)) == -1) {
		cout << "parse seat fail" << endl;
		return -1;
	} else {
		index += psize;
	}
	return index;
}

int Order::dump() {
	int index = 0;
	raw[index++] = 0xF6;
	int dsize;
	if ((dsize = Util::dump<unsigned char>(raw + index, RAW_SIZE - index, operation, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned int>(raw + index, RAW_SIZE - index, id, 4)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned char>(raw + index, RAW_SIZE - index, from, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<unsigned char>(raw + index, RAW_SIZE - index, to, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	if ((dsize = Util::dump<short>(raw + index, RAW_SIZE - index, seat, 2)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	int sum = Util::checksum(raw + 1, index - 1);
	if ((dsize = Util::dump<unsigned char>(raw + index, RAW_SIZE - index, sum, 1)) == -1) {
		return -1;
	} else {
		index += dsize;
	}
	raw[index++] = 0xF6;
	rsize = index;
	return index;
}
