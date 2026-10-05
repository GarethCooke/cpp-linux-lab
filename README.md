# cpp-linux-lab

Hands-on C++20 and Linux exercises for debugging and performance work: core dumps, sanitizers, deadlocks, false sharing, `perf`, and two from-memory coding exercises (a lock-free SPSC queue and a rule-of-five resource class).

Each lab is a small program that misbehaves in one specific way, plus a README that says what to run, what you should see, and how to fix it.

Tested on Ubuntu 24.04 with g++ 13.

## Setup

```bash
./setup.sh
```

This installs the toolchain (gcc, clang, gdb, valgrind, perf, bpftrace, rr, heaptrack) and sets the kernel parameters the labs need. The `sysctl` settings last until reboot, so run `./setup.sh --sysctl-only` after each reboot.

## Labs

| # | Lab | Skill | Time |
|---|-----|-------|------|
| 01 | [Segfault and core dump](labs/01-segfault-core) | `dmesg`, core files, `gdb` post-mortem, `addr2line` | 10 min |
| 02 | [ASan and UBSan](labs/02-asan-ubsan) | Use-after-free from iterator invalidation, signed overflow | 10 min |
| 03 | [TSan data race](labs/03-tsan-race) | Detecting and fixing a race; relaxed atomics | 10 min |
| 04 | [Deadlock](labs/04-deadlock) | `thread apply all bt`, finding a mutex owner, `std::scoped_lock` | 15 min |
| 05 | [False sharing](labs/05-false-sharing) | Measuring it, `perf c2c`, `alignas(64)` | 15 min |
| 06 | [perf and branch prediction](labs/06-perf-branch) | `perf stat`, `perf record`, flame graphs, `-O1` vs `-O3` | 20 min |
| 07 | [SPSC queue](labs/07-spsc-queue) | Write a lock-free ring buffer from memory; TSan stress test; benchmark | 45 min |
| 08 | [Rule of five](labs/08-rule-of-five) | Copy-and-swap, `std::exchange`, `noexcept` moves | 30 min |

Labs 07 and 08 are exercises: `make test` fails until you implement the header, and `make test IMPL=solution` runs the reference version.

## Build everything

```bash
make          # build all labs
make check    # run the reference solutions' tests under sanitizers (what CI runs)
make clean
```
