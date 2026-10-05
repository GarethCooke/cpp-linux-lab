// Lab 03, fixed: an atomic counter. Relaxed ordering is enough because no
// other memory is published through it; we only need each increment to be atomic.
#include <atomic>
#include <cstdio>
#include <thread>

std::atomic<long> fills{0};

int main() {
    {
        std::jthread a([] { for (int i = 0; i < 1'000'000; ++i) fills.fetch_add(1, std::memory_order_relaxed); });
        std::jthread b([] { for (int i = 0; i < 1'000'000; ++i) fills.fetch_add(1, std::memory_order_relaxed); });
    }
    std::printf("fills = %ld (expected 2000000)\n", fills.load());
}
