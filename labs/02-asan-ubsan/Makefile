SAN      := -fsanitize=address,undefined -fno-omit-frame-pointer
CXXFLAGS := -std=c++20 -O1 -g -Wall -Wextra

all: uaf uaf_fixed

uaf: uaf.cpp
	$(CXX) $(CXXFLAGS) $(SAN) $< -o $@

uaf_fixed: uaf_fixed.cpp
	$(CXX) $(CXXFLAGS) $(SAN) $< -o $@

clean:
	rm -f uaf uaf_fixed
