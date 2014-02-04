build:
	g++ -std=c++11 -I. -O2 -g -c util.cc -o util.o
	g++ -std=c++11 -I. -O2 -g -c train.cc -o train.o
	g++ -std=c++11 -I. -O2 -g -c bipbuffer.cc -o bipbuffer.o
	g++ -std=c++11 -I. -O2 -g -c connection.cc -o connection.o
	g++ -std=c++11 -I. -O2 -g -c order.cc -o order.o
	g++ -std=c++11 -I. -O2 -g -c server.cc -o server.o
	g++ -std=c++11 -I. -O2 -g -c benchmark_train.cc -o benchmark_train.o
	g++ -std=c++11 -I. -O2 -g -c benchmark_ringbuffer.cc -o benchmark_ringbuffer.o
	g++ -std=c++11 -I. -O2 -g -c benchmark_bipbuffer.cc -o benchmark_bipbuffer.o
	g++ -std=c++11 -I. -O2 -g -c benchmark_server.cc -o benchmark_server.o
	g++ -std=c++11 -I. -O2 -g -c test_util.cc -o test_util.o
	g++ -std=c++11 -I. -O2 -g -c test_order.cc -o test_order.o
	g++ -std=c++11 -I. -O2 -g -c test_train.cc -o test_train.o
	g++ -std=c++11 -I. -O2 -g -c test_ringbuffer.cc -o test_ringbuffer.o
	g++ -std=c++11 -I. -O2 -g -c test_bipbuffer.cc -o test_bipbuffer.o
	g++ -std=c++11 -I. -O2 -g -c test_server.cc -o test_server.o

benchmark:
	g++ -o benchmark_train ./train.o ./benchmark_train.o
	g++ -o benchmark_ringbuffer ./util.o ./benchmark_ringbuffer.o
	g++ -o benchmark_bipbuffer ./bipbuffer.o ./benchmark_bipbuffer.o
	g++ -o benchmark_server -pthread ./server.o ./benchmark_server.o ./connection.o ./bipbuffer.o ./util.o ./order.o

test:
	g++ -o test_util ./util.o ./test_util.o
	g++ -o test_order ./order.o ./test_order.o ./util.o
	g++ -o test_train ./train.o ./test_train.o
	g++ -o test_ringbuffer -pthread ./util.o ./test_ringbuffer.o
	g++ -o test_bipbuffer ./test_bipbuffer.o ./bipbuffer.o
	g++ -o test_server -pthread ./test_server.o ./server.o ./connection.o ./bipbuffer.o ./util.o ./order.o

temp:
	g++ -std=c++11 -I. -O2 -g -c util.cc -o util.o
	g++ -std=c++11 -I. -O2 -g -c order.cc -o order.o
	g++ -std=c++11 -I. -O2 -g -c test_order.cc -o test_order.o
	g++ -o test_order ./order.o ./test_order.o ./util.o

util:
	g++ -std=c++11 -I. -O2 -g -c util.cc -o util.o
	g++ -std=c++11 -I. -O2 -g -c test_util.cc -o test_util.o
	g++ -o test_util ./util.o ./test_util.o
