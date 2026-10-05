# 02 · ASan and UBSan

**Goal:** see two classic bugs that a normal build runs straight through, and read the sanitizer reports.

## Run

```bash
make
./uaf extra          # one argument, so argc == 2 and the addition overflows
./uaf_fixed extra
```

Then build the buggy version without sanitizers and run it:

```bash
g++ -std=c++20 -O2 uaf.cpp -o plain && ./plain extra
```

## What you should see

- UBSan reports first: `runtime error: signed integer overflow: 2 + 2147483646 cannot be represented in type 'int'`.
- Then ASan: `ERROR: AddressSanitizer: heap-use-after-free`, with three stacks: the bad read, where the memory was freed (inside `push_back`'s reallocation), and where it was first allocated.
- The plain build prints a number and exits 0. That is the point: undefined behaviour doesn't have to crash.

## Why

- `push_back` beyond capacity allocates a new buffer, moves the elements and frees the old one. Every reference, pointer and iterator into the old buffer is invalidated.
- Signed overflow is undefined, not wrapping. The optimiser is allowed to assume it never happens, which can rewrite loops in surprising ways. Unsigned arithmetic wraps by definition.

## Say it in the interview

"ASan and UBSan run on every CI build. They're cheap enough (about 2× CPU) to run the whole test suite under them. The classic finds are invalidated references after a container grows, and signed overflow."

## Stretch

- Run `valgrind ./plain extra`. It finds the use-after-free without recompiling, at 20–50× slowdown.
- Add `-fsanitize-recover=all` and `UBSAN_OPTIONS=halt_on_error=1` to make UBSan fatal in CI.
