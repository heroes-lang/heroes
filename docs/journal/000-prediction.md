# 000 — Prediction (author)

**Do not open `tools/spike/02-loop.c` until this file is filled in and
committed.** That file contains the answer; the value of this exercise is
recalling the shape yourself, not recognising it.

## The program

```
main = function: ()
    i: int @ 0
    total: int @ 0
    for i < 5
        total @ total + i
        i @ i + 1
    print(total)
```

## Your task

The lowering (M4) reduces ALL control flow to *basic blocks* — straight-line
runs of instructions — connected by jumps (unconditional) and conditional
jumps (two targets: taken / not taken). Draw that graph for the program above.

1. How many basic blocks? ______
2. For each block, list its instructions and where it jumps:

```
bb0: ________________________________   → ____
bb1: ________________________________   → ____ / ____
bb2: ________________________________   → ____
bb3: ________________________________   → ____
(add or remove blocks as you see fit — the count is YOUR prediction)
```

3. Which block does the loop's `for i < 5` test live in? ______
4. What does the program print? ______

## After committing this file

Open `tools/spike/02-loop.c`, compare, and record every divergence in
`000-setup.md` §3 — a wrong block count or a misplaced test is not a failure,
it IS the lesson (design.md Part 0).
