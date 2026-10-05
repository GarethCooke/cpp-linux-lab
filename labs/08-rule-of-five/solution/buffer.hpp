// Lab 08 reference solution: copy-and-swap.
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

    // The two constructors hold all the real work.
    Buffer(const Buffer& other) : Buffer(other.n_) {
        std::copy_n(other.p_, n_, p_);
    }
    Buffer(Buffer&& other) noexcept
        : n_(std::exchange(other.n_, 0)),
          p_(std::exchange(other.p_, nullptr)) {}   // source left empty, so its destructor is harmless

    // One operator covers copy AND move assignment. The argument is
    // copy-constructed from an lvalue or move-constructed from an rvalue at
    // the call site; swap takes the new state, and other's destructor frees
    // the old one. Strong exception guarantee; self-assignment is safe.
    // Trade-off: copy-assign always allocates, even if *this has room.
    Buffer& operator=(Buffer other) noexcept {
        swap(*this, other);
        return *this;
    }

    friend void swap(Buffer& a, Buffer& b) noexcept {
        using std::swap;
        swap(a.n_, b.n_);
        swap(a.p_, b.p_);
    }

    std::size_t size() const noexcept { return n_; }
    const double* data() const noexcept { return p_; }
    double& operator[](std::size_t i) noexcept { return p_[i]; }
    const double& operator[](std::size_t i) const noexcept { return p_[i]; }

private:
    std::size_t n_ = 0;
    double* p_ = nullptr;
};

}  // namespace lab
