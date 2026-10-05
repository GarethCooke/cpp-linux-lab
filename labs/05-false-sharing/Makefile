CXXFLAGS := -std=c++20 -O2 -g -fno-omit-frame-pointer -Wall -Wextra

all: fs

fs: fs.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f fs perf.data perf.data.old
