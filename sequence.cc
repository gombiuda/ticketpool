#include "sequence.h"

int Sequence::INITIAL_VALUE = -1L;

Sequence::Sequence() : Sequence(INITIAL_VALUE) {
}

Sequence::Sequence(long value_) : value(value_) {
}

long Sequence::get() {
	return value;
}

void Sequence::set(long value_) {
	value = value_;
}
