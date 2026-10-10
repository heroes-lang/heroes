---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: e381b16454a4c4a2c63aeb74ded1dc7fc177756c
github: none
---

- [ ] **555 — two groups declaring one C function with different types build, and the program prints a wrong value** | the critic's `clash`: `a.h` declares `long twice(long)`, `b.h` `double twice(double)`, a third module defines it; `check` and `build` exit 0 and `right.right(x: 4.0)` prints `4.0`, one C symbol called through two prototypes (C11 6.2.7p2, undefined behaviour with no diagnostic required); `test` refuses it | the FFI's view of one C name across groups, `selfhost/emit/` and `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0.

    Repaired at `e381b16454a4c4a2c63aeb74ded1dc7fc177756c`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every external name two header lists declare, bound by a group or not (panel 202's C1 with its widening, which refused no tracked program the bound names do not), is asked of each list in one question unit: clang's `aka` of a pointer to it is its canonical type and a refused `static` redeclaration its external linkage (`cli/two_ways_ask.hero`); two external declarations of two types are told at exit 1, `ffi_declared_two_ways`, naming both files and lines, before the link, in `build`, `run` and `test` (`cli/two_ways.hero`); a `static` definition is skipped, so `fp` builds and prints `6 8`. Cases `unsupported/fixedbugs-555-*` (2) and `run/fixedbugs-555-two-static-*`; unsupported 197 and 0, the compiler's own tests 1,546 passed; a warm self-build 389.4e9 instructions before and 390.7e9 after.
