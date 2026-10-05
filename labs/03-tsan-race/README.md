# 03 · TSan data race

**Goal:** catch a data race with ThreadSanitizer, then fix it with the cheapest correct atomic.

## Run

```bash
make
./race
./race_fixed
g++ -std=c++20 -O2 race.cpp -o plain && ./plain && ./plain && ./plain
```

If TSan aborts with `unexpected memory mapping`, run `sudo sysctl -w vm.mmap_rnd_bits=28` (setup.sh does this).

## What you should see

- `./race`: `WARNING: ThreadSanitizer: data race`, with the two conflicting accesses (both the `++fills` line) and the stacks of the threads that made them.
- `./plain`: often a total below 2,000,000, and different on each run. Sometimes exactly 2,000,000, because at `-O2` the compiler may turn each loop into a single `+= 1000000`. Both are allowed: a data race is undefined behaviour, not "a stale value".
- `./race_fixed`: no warning, always 2,000,000.

## Why relaxed is enough

`memory_order_relaxed` guarantees each increment is atomic but orders nothing else. Here nobody reads other data based on the counter, so that is all we need. The `jthread` joins provide the synchronisation for the final read.

## Say it in the interview

"Any unsynchronised write racing with another access is UB. For counters and statistics, relaxed atomics are correct and cheapest. TSan runs on the concurrency tests in CI. It's 5–15× slower and can't be combined with ASan, so it's a separate job."

## Stretch

- Watch the cache line bounce: `perf stat -e cycles,instructions ./race_fixed` (built without TSan), then give each thread its own counter and add them at the end.
