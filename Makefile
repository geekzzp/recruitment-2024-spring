CXX=g++
CXXFLAGS=-Og -g -fsanitize=address -Wall -Wextra -Wshadow -pipe

OPENMP_FLAGS=-fopenmp

all: main

main: baseline.o main.o solution.o
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $^

baseline.o: baseline.cc
	$(CXX) -O0 -Wall -Wextra -Wshadow -pipe -c -o $@ $^

solution.o: solution.cc
	$(CXX) -O3 -Wall -Wextra -Wshadow -pipe $(OPENMP_FLAGS) -c -o $@ $^

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -c -o $@ $^

clean:
	rm -f *.o main
