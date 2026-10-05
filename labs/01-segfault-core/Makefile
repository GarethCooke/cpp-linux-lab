CXXFLAGS := -std=c++20 -O2 -g -fno-omit-frame-pointer -Wall -Wextra

all: seg

seg: seg.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f seg core.*
