// Lab 07 reference: layer 1 only (correct, but head_ and tail_ share a
// cache line and every call reads the other thread's index).
// Used by bench.cpp as the baseline.
#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>

namespace lab_v1 {

template <typename T, std::size_t N>
    requires(std::has_single_bit(N))
class SpscQueue {
public:
    bool push(const T& value) {
        const auto t = tail_.load(std::memory_order_relaxed);
        if (t - head_.load(std::memory_order_acquire) == N) return false;
        buf_[t & (N - 1)] = value;
        tail_.store(t + 1, std::memory_order_release);
        return true;
    }

    bool pop(T& out) {
        const auto h = head_.load(std::memory_order_relaxed);
        if (h == tail_.load(std::memory_order_acquire)) return false;
        out = buf_[h & (N - 1)];
        head_.store(h + 1, std::memory_order_release);
        return true;
    }

private:
    std::array<T, N> buf_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};
};

}  // namespace lab_v1
