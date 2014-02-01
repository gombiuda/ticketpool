build:
	g++ -std=c++11 -I. -O2 -g -c util.cc -o util.o
	g++ -std=c++11 -I. -O2 -g -c train.cc -o train.o
	g++ -std=c++11 -I. -O2 -g -c ringbuffer.cc -o ringbuffer.o
	g++ -std=c++11 -I. -O2 -g -c benchmark.cc -o benchmark.o
	g++ -std=c++11 -I. -O2 -g -c test_train.cc -o test_train.o

benchmark:
	g++ -lrt -o benchmark ./train.o ./benchmark.o

test:
	g++ -lrt -o test_util ./util.o ./test_util.o
	g++ -lrt -o test_train ./train.o ./test_train.o
	g++ -lrt -o test_ringbuffer ./ringbuffer.o ./test_train.o
