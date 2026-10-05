# IMPL selects which spsc.hpp to build against:
#   make test                 your version in this folder
#   make test IMPL=solution   the reference solution
# Both targets always rebuild, so switching IMPL needs no clean.
IMPL     ?= .
CXXFLAGS := -std=c++20 -g -Wall -Wextra -I$(IMPL) -Isolution

.PHONY: all test bench clean

# build only; `make test` and `make bench` build and run
all:
	$(CXX) $(CXXFLAGS) -O1 -fsanitize=thread test.cpp -o test_tsan
	$(CXX) $(CXXFLAGS) -O2 -DNDEBUG bench.cpp -o bench -pthread

test:
	$(CXX) $(CXXFLAGS) -O1 -fsanitize=thread test.cpp -o test_tsan
	./test_tsan

bench:
	$(CXX) $(CXXFLAGS) -O2 -DNDEBUG bench.cpp -o bench -pthread
	./bench $(CPUS)

clean:
	rm -f test_tsan bench
