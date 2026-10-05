CXXFLAGS := -std=c++20 -g -fno-omit-frame-pointer -Wall -Wextra

all: br br_o1 br_o3

# Branch forced: -fno-if-conversion stops gcc turning the if into a cmov,
# so branch prediction decides the speed
br: br.cpp
	$(CXX) $(CXXFLAGS) -O1 -fno-if-conversion -fno-if-conversion2 $< -o $@

# What gcc does on its own at -O1: a branchless conditional move
br_o1: br.cpp
	$(CXX) $(CXXFLAGS) -O1 $< -o $@

# -O3: branchless and vectorised
br_o3: br.cpp
	$(CXX) $(CXXFLAGS) -O3 $< -o $@

clean:
	rm -rf br br_o1 br_o3 perf.data perf.data.old br.svg FlameGraph
