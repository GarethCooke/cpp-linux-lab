# 01 · Segfault and core dump

**Goal:** go from "the process died" to the exact line, using only what a production box gives you.

## Run

```bash
ulimit -c unlimited
make
./seg
sudo dmesg | tail -2
ls core.*
gdb ./seg core.seg.*
```

In gdb:

```
bt
frame 0
info locals
p o
p orders
```

## What you should see

- The shell prints `Segmentation fault (core dumped)`.
- `dmesg` shows a line like `seg[1234]: segfault at 8 ip 000055... error 4 in seg[...]`.
  - `at 8`: the faulting address is 8, which is `offsetof(Order, qty)` from a null pointer. A small address almost always means a null pointer plus a member offset.
  - `error 4`: a user-mode read of an unmapped page (bit 2 = user, bit 1 = write, bit 0 = protection fault).
- `bt` points at the `total += o->qty` line in `totalQty`, and `p o` prints `0x0`.

## Without a core file

Take the offset from the `dmesg` line (the `+0x...` inside the brackets), or work it out as `ip` minus the binary's load base:

```bash
addr2line -e seg -f -C 0x<offset>
```

## Say it in the interview

"First `dmesg` for the faulting address and instruction pointer. A small faulting address means a null member access. Then the core in gdb for the stack and locals. Release builds always carry `-g` and frame pointers, with the debug info split off, so a core is always usable."

## Stretch

- Run `gcore <pid>` against a sleeping process to snapshot a live process without killing it.
- Rebuild with `-O0` and compare the backtrace: with `-O2`, `totalQty` may be inlined into `main`.
