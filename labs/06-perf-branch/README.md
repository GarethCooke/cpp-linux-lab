# 06 · perf and branch prediction

**Goal:** use `perf` to explain a large speed difference between two runs of identical code, then see that the compiler can remove the difference on its own.

## Run

```bash
make
./br
./br sorted
perf stat -e cycles,instructions,branches,branch-misses ./br
perf stat -e cycles,instructions,branches,branch-misses ./br sorted
perf record -g ./br && perf report --no-children
perf annotate main
./flame.sh
./br_o1 && ./br_o1 sorted
./br_o3 && ./br_o3 sorted
objdump -d --no-show-raw-insn -C br_o1 | awk '/<main>:/,/^$/' | grep cmov
```

Each run prints the time of the loop only, so the sort isn't counted.

## What you should see

Measured with g++ 13 on x86-64:

| Binary | Unsorted | Sorted |
|---|---|---|
| `br` (branch forced) | ~1500 ms | ~170 ms |
| `br_o1` (plain `-O1`) | ~225 ms | ~245 ms |
| `br_o3` | ~150 ms | ~130 ms |

- `perf stat` on unsorted `./br`: `branch-misses` around 20–25% of branches and low IPC. Sorted: close to 0% misses and high IPC.
- `perf annotate`: the cost piles up on the instructions right after the `cmp`/`jl` pair. Sample attribution skids one or two instructions, so look at the neighbourhood, not the exact line.
- `br_o1`: sorted and unsorted run at the same speed, and the disassembly shows a `cmov` instead of a conditional jump. gcc applies if-conversion even at `-O1`; the `br` build turns it off to expose the branch.
- `br_o3`: faster again; look for vector instructions (`vpcmpgtd`, `vpaddq`, or `pcmpgtd`, `paddq`).

## Why

A mispredicted branch flushes the pipeline: about 15–20 cycles on modern x86. Random data makes the `x >= 128` branch a coin flip; sorted data makes it one long run of not-taken followed by one long run of taken. A conditional move has no branch to mispredict, so data order stops mattering.

## Say it in the interview

"`perf stat` first: IPC and branch-miss rate tell me whether I'm memory-bound or front-end-bound before I touch code. Then `perf record` and `annotate` to find the instruction. And I check the assembly before hand-optimising, because the compiler often already removes the branch, or would with PGO."

## Stretch

- Write a branchless version by hand (`sum += (x >= 128) * x;`), build it the same way as `br`, and compare.
- Try PGO on `br.cpp`: build with `-O2 -fprofile-generate`, run it, rebuild with `-O2 -fprofile-use`, and compare.
