// Lab 07 benchmark: throughput of your queue vs the layer-1 baseline.
// Usage: ./bench [producer_cpu consumer_cpu]
// Pick two different physical cores (check `lscpu -e`; hyperthread
// siblings share a core and hide the cache-line effects).
#include <spsc.hpp>
#include <spsc_v1.hpp>

#include <pthread.h>
#include <sched.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace {

constexpr std::uint64_t kItems = 50'000'000;
int producerCpu = -1;
int consumerCpu = -1;

void pin(int cpu) {
    if (cpu < 0) return;
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(cpu, &set);
    pthread_setaffinity_np(pthread_self(), sizeof(set), &set);
}

template <typename Queue>
double opsPerSecond() {
    auto q = std::make_unique<Queue>();
    std::uint64_t checksum = 0;
    const auto t0 = std::chrono::steady_clock::now();
    std::thread consumer([&] {
        pin(consumerCpu);
        std::uint64_t v;
        for (std::uint64_t n = 0; n < kItems;) {
            if (q->pop(v)) { checksum += v; ++n; }
        }
    });
    pin(producerCpu);
    for (std::uint64_t i = 0; i < kItems;) {
        if (q->push(i)) ++i;
    }
    consumer.join();
    const double secs = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    if (checksum != kItems * (kItems - 1) / 2) std::puts("  checksum mismatch: the queue lost or duplicated items");
    return kItems / secs;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc == 3) {
        producerCpu = std::atoi(argv[1]);
        consumerCpu = std::atoi(argv[2]);
    }
    std::printf("%llu items, producer cpu %d, consumer cpu %d (-1 = unpinned)\n",
                static_cast<unsigned long long>(kItems), producerCpu, consumerCpu);
    using Item = std::uint64_t;
    std::printf("  layer-1 baseline : %6.1f M ops/s\n", opsPerSecond<lab_v1::SpscQueue<Item, 4096>>() / 1e6);
    std::printf("  your queue       : %6.1f M ops/s\n", opsPerSecond<lab::SpscQueue<Item, 4096>>() / 1e6);
}
