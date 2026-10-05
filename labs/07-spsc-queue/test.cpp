// Lab 07 tests. Built with -fsanitize=thread by `make test`.
#include <spsc.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <thread>

namespace {

int failures = 0;

void check(bool ok, const char* what) {
    std::printf("  %s  %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++failures;
}

void testEmpty() {
    lab::SpscQueue<int, 8> q;
    int v = -1;
    check(!q.pop(v), "pop on an empty queue returns false");
}

void testCapacityAndOrder() {
    lab::SpscQueue<int, 8> q;
    bool allPushed = true;
    for (int i = 0; i < 8; ++i) allPushed &= q.push(i);
    check(allPushed, "all N = 8 slots accept a push");
    check(!q.push(99), "push N+1 on a full queue returns false");

    bool fifo = true;
    for (int i = 0; i < 8; ++i) {
        int v = -1;
        fifo &= q.pop(v) && v == i;
    }
    check(fifo, "pops come out in FIFO order");
    int v = -1;
    check(!q.pop(v), "queue is empty again after draining");
}

void testWrapAround() {
    lab::SpscQueue<int, 4> q;
    bool ok = true;
    for (int i = 0; i < 1000; ++i) {
        ok &= q.push(i) && q.push(i + 1);
        int a = -1, b = -1;
        ok &= q.pop(a) && q.pop(b) && a == i && b == i + 1;
    }
    check(ok, "1000 cycles round a 4-slot ring stay in order");
}

void testTwoThreads() {
    constexpr std::uint64_t kCount = 2'000'000;
    lab::SpscQueue<std::uint64_t, 1024> q;
    bool inOrder = true;

    std::thread consumer([&] {
        for (std::uint64_t expected = 0; expected < kCount;) {
            std::uint64_t v;
            if (q.pop(v)) {
                if (v != expected) inOrder = false;
                ++expected;
            }
        }
    });
    for (std::uint64_t i = 0; i < kCount;) {
        if (q.push(i)) ++i;
    }
    consumer.join();
    check(inOrder, "2,000,000 items across two threads arrive complete and in order");
}

}  // namespace

int main() {
    std::puts("SPSC queue tests");
    testEmpty();
    testCapacityAndOrder();
    testWrapAround();
    if (failures == 0) {
        testTwoThreads();
    } else {
        std::puts("  SKIP  two-thread test (fix the single-threaded failures first)");
    }
    std::printf("%s\n", failures == 0 ? "ALL PASSED" : "FAILED");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
