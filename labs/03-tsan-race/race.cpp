// Lab 03: two threads increment a shared counter with no synchronisation.
#include <cstdio>
#include <thread>

long fills = 0;

int main() {
    {
        std::jthread a([] { for (int i = 0; i < 1'000'000; ++i) ++fills; });
        std::jthread b([] { for (int i = 0; i < 1'000'000; ++i) ++fills; });
    }   // jthreads join here
    std::printf("fills = %ld (expected 2000000)\n", fills);
}
