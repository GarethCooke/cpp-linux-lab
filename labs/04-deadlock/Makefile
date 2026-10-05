CXXFLAGS := -std=c++20 -O0 -g -Wall -Wextra

all: dead dead_fixed

dead: dead.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

dead_fixed: dead_fixed.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f dead dead_fixed
