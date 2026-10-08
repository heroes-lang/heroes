---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: ac33242ee17328d03977307e580479fa446ca28c
github: none
---

- [x] **468 — Windows' clang dies in its front end on the options chain 10,000 deep, under every debug word** | `o10000` (variants and options 10,000 deep): exit 139 on the box at `-g`, line tables and `-g0` alike, stock and under panel 197's (E), clang warning *stack nearly exhausted* at a struct copy (panel 197's compiler-engineer; the phase split owed on the box) | past panel 184's R6 floor of 2,000 · panel 197's R7 · defect 219 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7 (`docs/panel/197-the-debug-word-follows-the-level-and-the-type-chain-reaches-the-debug-writer-from-its-bottom.md`).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor R6 sets.

    Repaired at `ac33242e`, 2026-10-08 (lane b15-deepc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The phase, split on the box (clang 23.1.1): `-fsyntax-only` passes, and clang's emission of `static` functions, which recurses as deep as the chain the first time one function names the next, warns *stack nearly exhausted* at R994's `_eq` and dies at `-c` under `-g` and `-g0`, defect 469's memmove or not; past 32 deep a unit now redefines `HERO_TU_LOCAL` with `used`, so each function is emitted at its definition, and the 10,000-deep unit compiles there at `-c` under `-g0`, `-g` and line tables with no warning, together with defect 469's memmove (8 s of front end; without it the front end alone is defect 469's hour), on this Mac the deepest of six samples across a 1,000-level unit held about 2,800 nested emissions, and three with `used`; Linux arm64 (Debian clang 22.1.8) dies at `-emit-llvm` of the 10,000-level unit under 1 and 2 MiB of stack and passes at 4 and 8, with `used` it passes at 1, 2, 4 and 8, and this Mac dies at 1 MiB without it and passes with it.

## The repair

Repaired at `ac33242e`, 2026-10-08 (lane b15-deepc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The phase, split on the box (clang 23.1.1): `-fsyntax-only` passes, and clang's emission of `static` functions, which recurses as deep as the chain the first time one function names the next, warns *stack nearly exhausted* at R994's `_eq` and dies at `-c` under `-g` and `-g0`, defect 469's memmove or not; past 32 deep a unit now redefines `HERO_TU_LOCAL` with `used`, so each function is emitted at its definition, and the 10,000-deep unit compiles there at `-c` under `-g0`, `-g` and line tables with no warning, together with defect 469's memmove (8 s of front end; without it the front end alone is defect 469's hour), on this Mac the deepest of six samples across a 1,000-level unit held about 2,800 nested emissions, and three with `used`; Linux arm64 (Debian clang 22.1.8) dies at `-emit-llvm` of the 10,000-level unit under 1 and 2 MiB of stack and passes at 4 and 8, with `used` it passes at 1, 2, 4 and 8, and this Mac dies at 1 MiB without it and passes with it.

**Closed 2026-10-09**, after the push's platform legs, this defect being at the C boundary or changing what clang gets on every platform (`.claude/rules/verification.md` § The batch): batch 15 closed on this Mac alone (`811f8398`) and the CI's legs ran its cases afterwards, run 37844345400, created at 23:06 on 2026-10-08 and its Windows leg finished at 01:31 on 2026-10-09. Darwin arm64 read the net 6,960 passed and 0 failed, Linux arm64 and Linux x86-64 6,941 each, and on those three legs the compiler's own tests 1,478, the module's 269 and the net's own tests 318, all passed; the Windows leg read the compiler's own tests 1,478 and the module's 269, all passed, and the net 6,811 passed and 1 failed, defect 322's sanitiser case alone (defect 509), its net's own tests not reached. This defect's cases are not among any leg's SKIP lines, read from the four logs, and passed on every leg; defect 447's vcpkg step ran on the Windows leg and its SDL3 case ran there, where batch 14's leg had skipped it. Defect 463's case is the hand-written one its lane ran on a Linux with SELinux enforcing, which no leg is.
