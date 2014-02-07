#pragma once

#include <vector>
#include "sequence.h"

using namespace std;

class Sequencer {
public:
	Sequencer(int size);
	template<typename... Sequences> void before(Sequences*... sequences);
	long get();
	long next(int n);
	void publish(int n);
	int get_size();
private:
	Sequence *cursor;
	vector<Sequence*> *befores;
	long next_value;
	int size;
};
