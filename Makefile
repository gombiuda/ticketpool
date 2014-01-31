build:
	g++ -std=c++11 -lrt *.cc

benchmark:
	g++ -std=c++11 -lrt -g -o benchmark *.cc
	./benchmark
