// Lab 07 exercise: implement a bounded single-producer single-consumer queue.
//
// Rules:
//   - exactly one thread calls push(), exactly one other thread calls pop()
//   - no locks, no allocation after construction
//   - N is a power of two; all N slots must be usable
//
// Build it in three layers (see README):
//   1. correct: two atomic indices with acquire/release
//   2. no false sharing: each index on its own cache line
//   3. cached indices: the fast path never reads the other thread's line
//
// make test        runs the tests under ThreadSanitizer against this file
// make bench       compares your queue with the reference layer-1 version
#pragma once

#include <array>
#include <atomic>
#include <bit>
#include <cstddef>

namespace lab {

template <typename T, std::size_t N>
    requires(std::has_single_bit(N))
class SpscQueue {
public:
    // Producer thread only. Returns false if the queue is full.
    bool push(const T& value) {
        (void)value;
        return false;   // TODO
    }

    // Consumer thread only. Returns false if the queue is empty.
    bool pop(T& out) {
        (void)out;
        return false;   // TODO
    }

private:
    // TODO: indices and storage
};

}  // namespace lab
