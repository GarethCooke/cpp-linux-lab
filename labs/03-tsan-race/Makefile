CXXFLAGS := -std=c++20 -O1 -g -Wall -Wextra -fsanitize=thread

all: race race_fixed

race: race.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

race_fixed: race_fixed.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f race race_fixed plain
