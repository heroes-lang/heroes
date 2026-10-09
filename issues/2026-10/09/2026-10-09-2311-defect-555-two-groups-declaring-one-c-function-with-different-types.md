---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **555 — two groups declaring one C function with different types build, and the program prints a wrong value** | the critic's `clash`: `a.h` declares `long twice(long)`, `b.h` `double twice(double)`, a third module defines it; `check` and `build` exit 0 and `right.right(x: 4.0)` prints `4.0`, one C symbol called through two prototypes (C11 6.2.7p2, undefined behaviour with no diagnostic required); `test` refuses it | the FFI's view of one C name across groups, `selfhost/emit/` and `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0.
