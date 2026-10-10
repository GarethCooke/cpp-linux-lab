# IMPL selects which buffer.hpp to build against:
#   make test                 your version in this folder
#   make test IMPL=solution   the reference solution
IMPL     ?= .
CXXFLAGS := -std=c++20 -O1 -g -Wall -Wextra -I$(IMPL) -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test debug clean

# build only; `make test` builds and runs
all:
	$(CXX) $(CXXFLAGS) test.cpp -o test

test:
	$(CXX) $(CXXFLAGS) test.cpp -o test
	./test

# unoptimised, no sanitizer: for stepping through in gdb / VS Code
debug:
	g++ -std=c++20 -O0 -g -Wall -Wextra -I$(IMPL) test.cpp -o test_dbg

clean:
	rm -f test test_dbg
