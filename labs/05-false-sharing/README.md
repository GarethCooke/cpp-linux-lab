# 05 · False sharing

**Goal:** measure false sharing, then find it with a tool rather than by reading code.

## Run

```bash
make
./fs
perf stat -e cycles,instructions,cache-misses ./fs
sudo perf c2c record ./fs
sudo perf c2c report --stdio | head -60
```

To make the effect sharper, pin the threads to two different physical cores:

```bash
taskset -c 0,2 ./fs
```

## What you should see

- `shared` takes several times longer than `padded` (about 5× on a typical x86 box). The threads never touch each other's counter.
- `perf stat`: much lower instructions per cycle (IPC) for the run overall. Cores sit stalled waiting for the line.
- `perf c2c report`: a "Shared Data Cache Line" table with one hot line showing a high HITM count (loads that hit a line Modified in another core's cache), and two offsets in it (0 and 8) written by different threads.

## Why

Caches work in 64-byte lines. To write, a core must own the line exclusively (MESI). Two cores writing different variables on one line keep stealing it from each other, so every increment becomes a cross-core transfer of 40–100 ns instead of an L1 hit of about 1 ns.

## Fix

`alignas(64)` per hot variable, or `alignas(std::hardware_destructive_interference_size)` from `<new>` (gcc warns that its value can vary by `-mtune`). Some teams pad to 128 bytes because Intel's adjacent-line prefetcher pulls lines in pairs.

## Say it in the interview

"Anything written by different threads goes on its own cache line: queue head and tail, per-thread counters. If I suspect it, `perf c2c` shows the contended line and the offsets being written."

## Stretch

- Change `Padded` to `alignas(128)` and compare.
- Give each thread a local counter and publish once at the end. That's the single-writer principle, and it beats both versions.
