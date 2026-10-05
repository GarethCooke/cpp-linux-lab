# 07 · SPSC queue

**Goal:** write a lock-free single-producer single-consumer ring buffer from memory, prove it correct under ThreadSanitizer, and measure what each optimisation buys.

## The task

Implement `push` and `pop` in [`spsc.hpp`](spsc.hpp). Don't look at `solution/` until your version passes.

```bash
make test                 # fails until you implement it
make test IMPL=solution   # the reference, for comparison
make bench                # your queue vs the layer-1 baseline
make bench CPUS="0 2"     # pin producer and consumer to cpus 0 and 2
```

Pick two different physical cores for `CPUS`: `lscpu -e` shows which cpus share a core.

## Build it in layers

**Layer 1: correct.** Two `std::atomic<std::size_t>` indices, `head_` (consumer writes) and `tail_` (producer writes), as monotonic counters masked with `N - 1` only when indexing.

- Each side loads its own index `relaxed`: nobody else writes it.
- `push` loads `head_` with `acquire` to check for full; `pop` loads `tail_` with `acquire` to check for empty.
- `push` writes the slot, then stores `tail_` with `release`. That publishes the data.
- `pop` reads the slot, then stores `head_` with `release`. That hands the slot back, so the producer can't overwrite it while it's still being read.

**Layer 2: no false sharing.** `alignas(64)` on `tail_`, `head_` and the buffer, so the producer's and the consumer's writes land on different cache lines.

**Layer 3: cached indices.** The producer keeps a private `cachedHead_` and reloads `head_` only when the queue *looks* full; the consumer does the same with `cachedTail_`. A stale copy is always behind the real value, so it can only make the queue look fuller or emptier than it is, never wrongly report space or data. On the common path neither thread touches the other's cache line.

## What you should see

- `make test`: all seven checks pass and TSan reports nothing. Try deliberately weakening one `release` to `relaxed` and running it again: TSan reports a data race on the buffer slot.
- `./bench`: layer 3 is typically several times faster than the layer-1 baseline when the threads are on different physical cores.

## Questions to answer out loud

1. Why is `seq_cst` unnecessary here, and what would it cost on x86?
2. Why does the full check use `t - head == N` rather than `(t + 1) % N == head`?
3. What breaks with two producers, and what would an MPSC version need?
4. How would you support a `T` that is expensive to copy or not default-constructible?

Brief answers: (1) no store-then-load pattern across two variables needs a single total order; a `seq_cst` store compiles to `xchg` on x86. (2) monotonic counters keep all N slots usable and make full and empty unambiguous. (3) two producers race on `tail_`; MPSC needs `fetch_add` or CAS to claim a slot, plus a per-slot sequence number. (4) raw aligned storage, placement `new` in an `emplace`, and destroy after moving out in `pop`.
