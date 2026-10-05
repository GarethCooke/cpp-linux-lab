# 04 · Deadlock

**Goal:** prove a lock-order deadlock from a hung process, the way you would on a production box.

## Run

```bash
make
./inspect.sh
./dead_fixed
```

`inspect.sh` starts `./dead`, attaches gdb with `sudo` (Ubuntu blocks attaching to a non-child process otherwise), runs `owners.gdb`, and kills the process.

To do it by hand:

```bash
./dead &
sudo gdb -p $(pgrep -n dead)
(gdb) info threads
(gdb) thread apply all bt
(gdb) p book._M_mutex.__data.__owner
(gdb) p risk._M_mutex.__data.__owner
(gdb) detach
kill %1
```

## What you should see

- Two worker threads, each with `__lll_lock_wait` / `pthread_mutex_lock` at the top of its stack and the `lock_guard` line of `dead.cpp` below.
- The two `__owner` values are thread LWP numbers. Match them against `info threads`: the matcher holds `book` and waits on `risk`, the limits thread holds `risk` and waits on `book`. That cycle is your proof.

`std::mutex` wraps a glibc `pthread_mutex_t` in `_M_mutex`. For a raw `pthread_mutex_t` it's `p m.__data.__owner`.

## Fixes, in order of preference

1. Don't hold two locks at once: redesign so each piece of data has one owner thread.
2. Take multiple locks with `std::scoped_lock(a, b)`, which avoids deadlock whatever order you name them in.
3. Enforce one global lock order and document it. TSan reports `lock-order-inversion` when it's violated, even on runs that didn't deadlock.

## Say it in the interview

"Attach and dump every thread's stack. Threads in `pthread_mutex_lock` are blocked, and glibc records the owner thread in the mutex, so I follow owner to waiter until it cycles. The fix is `scoped_lock` or a lock-ordering rule, and TSan catches inversions in CI before they ever hang."

## Stretch

- Build `dead.cpp` with `-fsanitize=thread` and run it. TSan reports the lock-order inversion before it hangs.
- Try `eu-stack -p <pid>` (from elfutils) as a faster, non-interactive stack dump.
