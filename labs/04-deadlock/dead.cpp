// Lab 04: two threads take the same two mutexes in opposite orders.
#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

std::mutex book;   // protects the order book
std::mutex risk;   // protects risk limits

int main() {
    using namespace std::chrono_literals;
    std::jthread matcher([] {
        std::lock_guard l1(book);
        std::this_thread::sleep_for(50ms);   // widen the window
        std::lock_guard l2(risk);
    });
    std::jthread limits([] {
        std::lock_guard l1(risk);
        std::this_thread::sleep_for(50ms);
        std::lock_guard l2(book);
    });
    std::puts("started; this never finishes");
}
