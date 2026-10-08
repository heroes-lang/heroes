---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: 0c930f3d6375253ff192d75605c2ecc7f39a1baf
github: none
---

- [x] **263 — the emitter writes a `#line` for every slot a return releases, most of which no statement follows** | 62,723 of `slots-returns-100`'s 97,108 lines of C are such `#line`s (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`); the sweep itself is defect 231's | the emitter's release sweep at a return (`selfhost/emit/`), beside defect 231 · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): C the compiler writes and clang reads for nothing; no program refused or wrong.

    Measured 2026-10-07 by lane b14-ir at `dad2da47`, not repaired there: since defect 231's one exit, `slots-returns-100` (the shape rebuilt from its description) emits 7,533 lines of C and 3,133 `#line`s, 301 of them followed by another `#line`, and the compiler's own C 35,957 such of 532,475. They are the emitter's, not the IR's: `emit/inst.hero` writes the instruction's `#line` before every instruction, and a retain or a release is written under the generated file's (`reference_named`), so a release after a release gets a source `#line` no statement follows whatever line the IR gives it; defect 335's repair gave the sweep the `return`'s line and the count stayed 3,133. A probe built from a copy of the tree that skips that `#line` for `incref`, `decref` and `decref_slot` wrote 2,531 `#line`s and none dead at `slots-returns-100`, every other line identical, and 461,299 with 81 dead for its own C, 1,287,591 lines where the seed holds 1,358,691; the repair is the emit lane's file.

    Repaired at `0c930f3d`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The writer keeps an `at_file` or `at_generated` request pending until a line is written under it, so one another replaces or the output ends on is never written, and one the claim already holds is not stated again: `slots-returns-100` reads 2,527 `#line`s and none dead where it read 3,133 and 301, the compiler's own C 448,462 and none where it read 533,919 and 35,956 (1,362,516 lines to 1,277,059, 41.3 MB to 38.5 MB), every other line the same, and lldb steps the same `.hero` lines in the same order; the bless is defect 335's residual's commit, `d15ab3fb`.

## The repair

Repaired at `0c930f3d`, 2026-10-07 (lane b14-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The writer keeps an `at_file` or `at_generated` request pending until a line is written under it, so one another replaces or the output ends on is never written, and one the claim already holds is not stated again: `slots-returns-100` reads 2,527 `#line`s and none dead where it read 3,133 and 301, the compiler's own C 448,462 and none where it read 533,919 and 35,956 (1,362,516 lines to 1,277,059, 41.3 MB to 38.5 MB), every other line the same, and lldb steps the same `.hero` lines in the same order; the bless is defect 335's residual's commit, `d15ab3fb`.

**Closed 2026-10-08**, after the push's platform legs, this defect being at the C boundary (`.claude/rules/verification.md` § The batch): batch 14 closed on this Mac alone and the CI's legs ran its cases afterwards. The CI's four legs on `ee95a6f0` (run 37735987684, created at 08:08 and its Windows leg finished at 10:22 on 2026-10-08) are all green: Darwin arm64 with the net at 6,809 passed and 0 failed, Linux arm64 and Linux x86-64 at 6,790 each, Windows x86-64 at 6,648, and on every leg the compiler's own tests 1,430, the module's 260 and the net's own tests 308, all passed. A case bound to one platform ran where it is bound, read from the legs' logs: the SDL3 event case of defect 213 and the `sys/prctl.h` case of defect 437 are not among the SKIP lines of either Linux leg (they are, as they must be, on Darwin and on Windows), and the Linux legs built SDL3 from source and checked that `pkg-config` answers 3.2.10 before the net started. The leg that had read red on `02256c1e`, Windows, did so on defect 505's test of the order of legs, repaired at `ee95a6f0`.
