// Lab 02, fixed.
#include <climits>
#include <cstdint>
#include <cstdio>
#include <vector>

int main(int argc, char**) {
    std::vector<int> fills{100, 200, 300};
    fills.reserve(4);             // no reallocation, or take the reference after push_back
    fills.push_back(400);
    const int first = fills[0];   // copy the value instead of holding a reference

    const std::int64_t x = std::int64_t{INT_MAX} - 1 + argc;   // widen before adding
    std::printf("%lld\n", static_cast<long long>(first + x));
}
