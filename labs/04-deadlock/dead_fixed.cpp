// Lab 04, fixed: std::scoped_lock acquires several mutexes with a
// deadlock-avoidance algorithm, so the order you name them in doesn't matter.
#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

std::mutex book;
std::mutex risk;

int main() {
    using namespace std::chrono_literals;
    {
        std::jthread matcher([] { std::scoped_lock l(book, risk); std::this_thread::sleep_for(50ms); });
        std::jthread limits([] { std::scoped_lock l(risk, book); std::this_thread::sleep_for(50ms); });
    }
    std::puts("finished");
}
