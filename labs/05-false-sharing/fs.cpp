// Lab 05: two threads each increment their OWN counter. When the counters
// share a 64-byte cache line, the line ping-pongs between cores.
#include <atomic>
#include <chrono>
#include <cstdio>
#include <thread>

struct Shared {                          // a and b on the same cache line
    std::atomic<long> a{0};
    std::atomic<long> b{0};
};

struct Padded {                          // a and b on separate cache lines
    alignas(64) std::atomic<long> a{0};
    alignas(64) std::atomic<long> b{0};
};

template <typename S>
double run() {
    S s;
    const auto t0 = std::chrono::steady_clock::now();
    {
        std::jthread x([&] { for (int i = 0; i < 50'000'000; ++i) s.a.fetch_add(1, std::memory_order_relaxed); });
        std::jthread y([&] { for (int i = 0; i < 50'000'000; ++i) s.b.fetch_add(1, std::memory_order_relaxed); });
    }
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
}

int main() {
    std::printf("sizeof(Shared) = %zu, sizeof(Padded) = %zu\n", sizeof(Shared), sizeof(Padded));
    std::printf("shared %.2f s   padded %.2f s\n", run<Shared>(), run<Padded>());
}
