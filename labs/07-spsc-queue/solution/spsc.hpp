// Lab 07 reference solution: layer 3 (correct, padded, cached indices).
#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>

namespace lab {

template <typename T, std::size_t N>
    requires(std::has_single_bit(N))
class SpscQueue {
    static constexpr std::size_t kMask = N - 1;

public:
    bool push(const T& value) {
        const auto t = tail_.load(std::memory_order_relaxed);          // only we write tail_
        if (t - cachedHead_ == N) {                                     // looks full
            cachedHead_ = head_.load(std::memory_order_acquire);       // consumer finished reading?
            if (t - cachedHead_ == N) return false;                     // really full
        }
        buf_[t & kMask] = value;
        tail_.store(t + 1, std::memory_order_release);                 // publish the slot
        return true;
    }

    bool pop(T& out) {
        const auto h = head_.load(std::memory_order_relaxed);          // only we write head_
        if (h == cachedTail_) {                                         // looks empty
            cachedTail_ = tail_.load(std::memory_order_acquire);       // producer published more?
            if (h == cachedTail_) return false;                         // really empty
        }
        out = buf_[h & kMask];
        head_.store(h + 1, std::memory_order_release);                 // hand the slot back
        return true;
    }

private:
    // Indices are monotonic counters, masked only when indexing:
    // tail - head is always the item count, so full (== N) and empty (==)
    // are unambiguous and all N slots are usable.
    alignas(64) std::atomic<std::size_t> tail_{0};   // written by producer
    std::size_t cachedHead_{0};                       // producer-private copy of head_
    alignas(64) std::atomic<std::size_t> head_{0};   // written by consumer
    std::size_t cachedTail_{0};                       // consumer-private copy of tail_
    alignas(64) std::array<T, N> buf_{};
};

}  // namespace lab
