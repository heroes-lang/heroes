---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: ac33242ee17328d03977307e580479fa446ca28c
github: none
---

- [ ] **468 — Windows' clang dies in its front end on the options chain 10,000 deep, under every debug word** | `o10000` (variants and options 10,000 deep): exit 139 on the box at `-g`, line tables and `-g0` alike, stock and under panel 197's (E), clang warning *stack nearly exhausted* at a struct copy (panel 197's compiler-engineer; the phase split owed on the box) | past panel 184's R6 floor of 2,000 · panel 197's R7 · defect 219 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7 (`docs/panel/197-the-debug-word-follows-the-level-and-the-type-chain-reaches-the-debug-writer-from-its-bottom.md`).

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): beyond the floor R6 sets.

    Repaired at `ac33242e`, 2026-10-08 (lane b15-deepc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The phase, split on the box (clang 23.1.1): `-fsyntax-only` passes, and clang's emission of `static` functions, which recurses as deep as the chain the first time one function names the next, warns *stack nearly exhausted* at R994's `_eq` and dies at `-c` under `-g` and `-g0`, defect 469's memmove or not; past 32 deep a unit now redefines `HERO_TU_LOCAL` with `used`, so each function is emitted at its definition, and the 10,000-deep unit compiles there at `-c` under `-g0`, `-g` and line tables with no warning (8 s of front end with 469's memmove), its nested emissions 3 on this Mac where they were 2,798 at 1,000 levels.
