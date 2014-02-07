CXXFLAGS=-O2 -std=c++0x -g -c -Wall -pthread

SRCS = util.cc train.cc bipbuffer.cc connection.cc order.cc server.cc sequence.cc sequencer.cc
OBJS = $(SRCS:.cc=.o)

TESTS = test_util test_train test_order test_ringbuffer test_bipbuffer test_server

all: $(OBJS) test benchmark

test_util: $(OBJS) test_util.o
	g++ -o test_util test_util.o $(OBJS)

test_train: $(OBJS) test_train.o
	g++ -o test_train test_train.o $(OBJS)

test_order: $(OBJS) test_order.o
	g++ -o test_order test_order.o $(OBJS)

test_ringbuffer: $(OBJS) test_ringbuffer.o
	g++ -pthread -o test_ringbuffer test_ringbuffer.o $(OBJS)

test_bipbuffer: $(OBJS) test_bipbuffer.o
	g++ -o test_bipbuffer test_bipbuffer.o $(OBJS)

test_server: $(OBJS) test_server.o
	g++ -pthread -o test_server test_server.o $(OBJS)

benchmark: $(OBJS) benchmark.o
	g++ -pthread -o benchmark benchmark.o $(OBJS)

benchmark_sequence: $(OBJS) benchmark_sequence.o
	g++ -pthread -o benchmark_sequence benchmark_sequence.o $(OBJS)

benchmark_train: $(OBJS) benchmark_train.o
	g++ -o benchmark_train benchmark_train.o $(OBJS)

benchmark_bipbuffer: $(OBJS) benchmark_bipbuffer.o
	g++ -o benchmark_bipbuffer benchmark_bipbuffer.o $(OBJS)

benchmark_ringbuffer: $(OBJS) benchmark_ringbuffer.o
	g++ -pthread -o benchmark_ringbuffer benchmark_ringbuffer.o $(OBJS)

benchmark_server: $(OBJS) benchmark_server.o
	g++ -pthread -o benchmark_server benchmark_server.o $(OBJS)

tests: $(TESTS)
	for test in $^ ; do \
	    ./$$test ; \
	done

clean:
	rm -f $(TESTS) *.o
