#include "sequencer.h"

Sequencer::Sequencer(int size_) : size(size_){
	cursor = new Sequence();
	next_value = cursor->get();
}

long Sequencer::get() {
	return cursor->get();
}

long Sequencer::next(int n) {
	//TODO: should check befores, do not overwrite them
	next_value += n;
	return next_value;
}

void Sequencer::publish(int n) {
	cursor->set(n);
}

int Sequencer::get_size() {
	return size;
}

template<typename... Sequences> void before(Sequences*... sequences) {
	// TODO: not implemented yet
}
