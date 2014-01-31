build:
	g++ -std=c++11 -lrt *.cc

benchmark:
	g++ -std=c++11 -lrt -o benchmark benchmark.cc
	./benchmark
