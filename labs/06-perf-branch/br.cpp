// Lab 06: the same loop over random vs sorted data. Run with any argument
// to sort first, which makes the branch predictable. Only the loop is timed.
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>

int main(int argc, char**) {
    std::vector<int> v(1 << 24);
    std::mt19937 gen(42);
    for (auto& x : v) x = static_cast<int>(gen() % 256);
    if (argc > 1) std::ranges::sort(v);

    const auto t0 = std::chrono::steady_clock::now();
    long sum = 0;
    for (int rep = 0; rep < 20; ++rep)
        for (int x : v)
            if (x >= 128) sum += x;      // taken about 50% of the time, at random unless sorted
    const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();

    std::printf("%s: loop %.0f ms (sum %ld)\n", argc > 1 ? "sorted  " : "unsorted", ms, sum);
}
