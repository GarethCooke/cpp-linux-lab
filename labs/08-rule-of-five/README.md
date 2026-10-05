# 08 · Rule of five

**Goal:** write the special member functions for a resource-owning class from memory, the way you'd do it on a whiteboard, and have sanitizers check the result.

## The task

In [`buffer.hpp`](buffer.hpp), replace the four `= delete` lines so that `Buffer`:

- copies deeply (its own allocation, contents copied)
- moves by taking the source's pointer and leaving the source empty
- has `noexcept` moves, so `std::vector` moves it instead of copying when it grows

The target shape is **two constructors plus one by-value assignment operator plus a `swap`**. You don't need all four declarations: one `operator=(Buffer)` taken by value covers both copy and move assignment. Declaring a separate `operator=(Buffer&&)` as well makes rvalue assignment ambiguous.

```bash
make test                 # your version
make test IMPL=solution   # the reference
```

## What the tests check

Type traits (copyable, `noexcept` moves), deep copy, self-assignment, moved-from state, buffer stealing, and `std::vector` growth. Everything runs under ASan, UBSan and LeakSanitizer, so a leaked old buffer in assignment or a double `delete[]` after a move fails the run even if every check prints PASS.

## Things to get right

- **Reset the source in the move constructor.** `std::exchange(other.p_, nullptr)` takes the pointer and nulls the source in one expression. Without the null, the moved-from destructor frees the buffer you just took.
- **Build assignment from the constructors, never the reverse.** A copy constructor written as `*this = other;` default-initialises the members first and then runs assignment against an object that may hold garbage.
- **`noexcept` is checked by declaration, not inferred.** `std::is_nothrow_move_constructible_v<Buffer>` looks only at what you declared. If a `noexcept` function throws anyway, the program calls `std::terminate`.

## Say it in the interview

"Rule of five means deciding all five, each written, defaulted or deleted. Prefer rule of zero, where the members manage themselves. When I do own a resource, the constructors do the work and assignment is copy-and-swap: strong exception guarantee, safe self-assignment, one place to maintain. The cost is that copy assignment always allocates, so on a hot path I'd hand-write it to reuse existing capacity, which is what `std::vector` does."

## Stretch

- Hand-write a copy assignment that reuses the existing buffer when `size()` already matches, keep the move assignment, and check the tests still pass.
- Make `Buffer` move-only: delete the copies, keep the moves, and change the copy tests to check `!std::is_copy_constructible_v`.
