---
kind: task
area: golden
milestone: M-generated-programs
filed: 2026-09-06
commit: none
github: none
---

- [ ] **M-generated-programs** | the catalogue of shapes, enumerated from the world | `tests/golden/fixedbugs/` · `docs/work/DONE.md`, the defect entries

    **Origin:** scheduled with the chain row, 2026-09-06, out of the author's
    instruction that the hunt look at the bugs found in other similar
    compilers. At its first step, and this item is a starting point rather than
    the list.

    **What one search found the day the row entered.** Csmith (Utah) generates
    random C programs and found hundreds of latent defects in GCC and LLVM by
    differential testing; YARPGen (Intel) generates programs free of undefined
    behaviour and reported **more than 220** bugs to GCC, LLVM and the Intel
    compiler, its own stated contribution being *generation policies* for
    diversity; a survey of compiler fuzzing exists (arXiv 2306.06884) and an
    OOPSLA'19 study asks how much the bugs found this way matter in practice;
    Zig carries an issue titled *Compiler crashes found with fuzzing*.

    **The nearest corpus is this repository, and it was measured rather than
    recalled**: **fourteen** defect entries in `docs/work/DONE.md`, **76**
    regression cases named after one (49 with the `fixedbugs-` prefix under
    `check/`, `run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`), and
    **63** points in `selfhost/` where the compiler declares a case impossible.

    **What is owed**: a `docs/measurements/` file naming each shape with its
    source and marking what was read and what was not, because CLAUDE.md §1 says
    an enumeration carries where it came from — and the two shapes the author
    named, the C boundary and depth, enter it with a number beside them rather
    than as an impression.

    **Where to look also:** `https://dl.acm.org/doi/10.1145/3428264` ·
    `https://arxiv.org/pdf/2306.06884` ·
    `https://github.com/ziglang/zig/issues/10121`.
    **Why it matters:** a generator aimed at the shapes one session can think of
    measures that session, and the whole point of the milestone is to be aimed
    at the world.

    **Re-verified 2026-09-10: STILL OPEN, and every count it states is now
    LOW.** `docs/work/DONE.md` carries **19** numbered defect entries, not fourteen —
    18 distinct ids, since 014 was issued twice and the next is 025
    (`docs/work/DEFECTS.md:20-26`). Regression cases are **58** `fixedbugs-*.hero`
    under `tests/golden/` (17 check, 35 run, 6 unsupported) against the item's 49, and
    **28** in `tests/golden/fixedbugs/` against 27: **86** in total, not 76. **And the
    63 is UNSETTLED**: no command reproduces it. Over `selfhost/**/*.hero`,
    `impossible` is 9, `cannot happen` 4, `internal error` 27 and `unreachable`
    **119**, so the catalogue's own first act is to define what it counts before it
    counts it.
