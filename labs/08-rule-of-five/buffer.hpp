// Lab 08 exercise: a resource-owning buffer of doubles.
//
// Make Buffer copyable (deep copy) and movable (steal the pointer), with:
//   - the copy constructor and move constructor doing the real work
//   - ONE assignment operator, by value, using copy-and-swap
//   - noexcept on the moves and on swap
//   - std::exchange in the move constructor
//
// Replace the four "= delete" lines below. `make test` checks your version
// under AddressSanitizer and UBSan, including for leaks and double frees.
#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>

namespace lab {

class Buffer {
public:
    Buffer() = default;
    explicit Buffer(std::size_t n) : n_(n), p_(new double[n]{}) {}
    ~Buffer() { delete[] p_; }

    // TODO: implement these
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    std::size_t size() const noexcept { return n_; }
    const double* data() const noexcept { return p_; }
    double& operator[](std::size_t i) noexcept { return p_[i]; }
    const double& operator[](std::size_t i) const noexcept { return p_[i]; }

private:
    std::size_t n_ = 0;
    double* p_ = nullptr;
};

}  // namespace lab
