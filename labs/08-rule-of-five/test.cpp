// Lab 08 tests. Built with -fsanitize=address,undefined by `make test`,
// so leaks, double frees and use-after-free fail the run too.
#include <buffer.hpp>

#include <cstdio>
#include <cstdlib>
#include <type_traits>
#include <utility>
#include <vector>

namespace {

int failures = 0;

void check(bool ok, const char* what) {
    std::printf("  %s  %s\n", ok ? "PASS" : "FAIL", what);
    if (!ok) ++failures;
}

// Templates so the bodies are only instantiated when the operation exists:
// the skeleton (with deleted members) still compiles and reports FAILs.
template <typename B>
void testTraits() {
    check(std::is_copy_constructible_v<B>, "copy constructible");
    check(std::is_copy_assignable_v<B>, "copy assignable");
    check(std::is_nothrow_move_constructible_v<B>, "move constructor is noexcept");
    check(std::is_nothrow_move_assignable_v<B>, "move assignment is noexcept");
}

template <typename B>
void testCopy() {
    if constexpr (std::is_copy_constructible_v<B> && std::is_copy_assignable_v<B>) {
        B a(3);
        a[0] = 1.5;
        B b(a);
        b[0] = 9.0;
        check(a[0] == 1.5 && b.size() == 3 && a.data() != b.data(), "copy construction is deep");

        B c(1);
        c = a;
        check(c.size() == 3 && c[0] == 1.5 && c.data() != a.data(), "copy assignment replaces and is deep");

        B& alias = a;
        a = alias;   // self-assignment through an alias
        check(a.size() == 3 && a[0] == 1.5, "self-assignment keeps the contents");
    } else {
        check(false, "copy tests (needs copy constructor and copy assignment)");
    }
}

template <typename B>
void testMove() {
    if constexpr (std::is_move_constructible_v<B> && std::is_move_assignable_v<B>) {
        B a(4);
        a[3] = 7.0;
        const double* p = a.data();
        B b(std::move(a));
        check(b.data() == p && b[3] == 7.0, "move construction steals the buffer");
        check(a.size() == 0 && a.data() == nullptr, "moved-from object is left empty");

        B c(2);
        c = std::move(b);   // c's old 2-element buffer must be freed (LeakSanitizer checks)
        check(c.data() == p && c.size() == 4, "move assignment steals the buffer");

        std::vector<B> v;
        for (int i = 0; i < 100; ++i) v.emplace_back(8);
        check(v.size() == 100 && v[99].size() == 8, "vector growth keeps every element intact");
    } else {
        check(false, "move tests (needs move constructor and move assignment)");
    }
}

}  // namespace

int main() {
    std::puts("Buffer tests");
    testTraits<lab::Buffer>();
    testCopy<lab::Buffer>();
    testMove<lab::Buffer>();
    std::printf("%s\n", failures == 0 ? "ALL PASSED" : "FAILED");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
