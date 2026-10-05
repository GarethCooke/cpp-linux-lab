// Lab 02: two bugs a sanitizer finds and a normal run usually hides.
#include <climits>
#include <cstdio>
#include <vector>

int main(int argc, char**) {
    std::vector<int> fills{100, 200, 300};
    int& first = fills[0];        // reference into the vector's buffer
    fills.push_back(400);         // reallocation frees that buffer: 'first' now dangles

    int x = INT_MAX - 1 + argc;   // signed overflow when run with one argument
    std::printf("%d\n", first + x);
}
