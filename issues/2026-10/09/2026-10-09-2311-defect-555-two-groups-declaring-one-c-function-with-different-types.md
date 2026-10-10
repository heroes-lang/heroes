---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: e381b16454a4c4a2c63aeb74ded1dc7fc177756c
github: none
---

- [x] **555 — two groups declaring one C function with different types build, and the program prints a wrong value** | the critic's `clash`: `a.h` declares `long twice(long)`, `b.h` `double twice(double)`, a third module defines it; `check` and `build` exit 0 and `right.right(x: 4.0)` prints `4.0`, one C symbol called through two prototypes (C11 6.2.7p2, undefined behaviour with no diagnostic required); `test` refuses it | the FFI's view of one C name across groups, `selfhost/emit/` and `selfhost/cli/` · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0.

    Repaired at `e381b16454a4c4a2c63aeb74ded1dc7fc177756c`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every external name two header lists declare, bound by a group or not (panel 202's C1 with its widening, which refused no tracked program the bound names do not), is asked of each list in one question unit: clang's `aka` of a pointer to it is its canonical type and a refused `static` redeclaration its external linkage (`cli/two_ways_ask.hero`); two external declarations of two types are told at exit 1, `ffi_declared_two_ways`, naming both files and lines, before the link, in `build`, `run` and `test` (`cli/two_ways.hero`); a `static` definition is skipped, so `fp` builds and prints `6 8`. Cases `unsupported/fixedbugs-555-*` (2) and `run/fixedbugs-555-two-static-*`; unsupported 197 and 0, the compiler's own tests 1,546 passed; a warm self-build 389.4e9 instructions before and 390.7e9 after.

    Its walks over a map's keys say their order at `8a29f2183703b25b9a61033ac90e1a4d9b3e5b48`, 2026-10-10, after the coordinator's trial net on batch 18's round read `order` red on them (design.md §4.9): in `cli/two_ways.hero` the names two lists share are sorted before a diagnostic reads them, and the counts there and the answers' table in `cli/two_ways_ask.hero` are marked as observing no order.

## The repair

Repaired at `e381b16454a4c4a2c63aeb74ded1dc7fc177756c`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every external name two header lists declare, bound by a group or not (panel 202's C1 with its widening, which refused no tracked program the bound names do not), is asked of each list in one question unit: clang's `aka` of a pointer to it is its canonical type and a refused `static` redeclaration its external linkage (`cli/two_ways_ask.hero`); two external declarations of two types are told at exit 1, `ffi_declared_two_ways`, naming both files and lines, before the link, in `build`, `run` and `test` (`cli/two_ways.hero`); a `static` definition is skipped, so `fp` builds and prints `6 8`. Cases `unsupported/fixedbugs-555-*` (2) and `run/fixedbugs-555-two-static-*`; unsupported 197 and 0, the compiler's own tests 1,546 passed; a warm self-build 389.4e9 instructions before and 390.7e9 after.

**Closed 2026-10-10** with batch 18 (lanes b18-close, b18-infer, b18-ffi and b18-guard, merged into the round `lane-round-b18` with the trunk), its closing gate run on the round at `f6528c53`: the seed regenerated over two generations, the runtime's ABI at 30, 50,640,450 bytes, SHA-256 beginning `3bfbddd0f618b118`, its fixpoint by `cmp`; the compiler's own tests 1,577, all passed; the net's own tests 332, all passed; the full net 7,872 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `order` on a walk of defect 570's `cli/pragma_ask.hero` with no `# ORDER:` mark, the mark written on its function's doc line (no line moved, the fixpoint re-checked by `cmp`) and `order` 3 and 0 after; eight floors told outgrown and raised in the closing commit, `order`, `runtime`, `emit`, `unsupported` and `probe` 3, 8, 12, 237 and 27, all 0 failed, after it. Defect 558's case, the one emission this Mac skips, was blessed and read green on Linux arm64 at the same commit (1,169 and 0). Under the optimistic chain the census and panel 187's R2 run after the push beside the CI, and a CI leg red on a closed defect's case files a new `blocking` defect naming it.
