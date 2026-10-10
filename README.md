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

## VS Code

Open the repo folder with `code .` and install the recommended C/C++ extension when prompted. Every task and debug configuration works on the lab folder of the file you have open.

| To | Do |
|----|----|
| Build the lab | `Ctrl+Shift+B` |
| Run the lab's tests (07, 08) | Terminal > Run Task > `lab: test` |
| Step through a lab program (01–06) | Open its `.cpp`, then Run and Debug > `Debug open lab program` > `F5` |
| Step through your exercise (07, 08) | Open `test.cpp`, then `Debug exercise tests` > `F5` (built at `-O0`, no sanitizers) |
| Inspect lab 01's crash | `Open core dump (01)`, then give the path to `core.seg.<pid>` |
| See lab 04's deadlock | Start `./dead` in the terminal, then `Attach to running process (04)` and pick `dead` |

The terminal commands in each lab's README are still the ones to practise for the interview. VS Code's debugger runs gdb underneath, and the Debug Console accepts gdb commands prefixed with `-exec`, e.g. `-exec thread apply all bt`.
