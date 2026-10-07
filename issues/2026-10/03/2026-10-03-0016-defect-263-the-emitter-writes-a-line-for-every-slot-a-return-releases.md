---
kind: defect
area: emit
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **263 — the emitter writes a `#line` for every slot a return releases, most of which no statement follows** | 62,723 of `slots-returns-100`'s 97,108 lines of C are such `#line`s (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`); the sweep itself is defect 231's | the emitter's release sweep at a return (`selfhost/emit/`), beside defect 231 · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): C the compiler writes and clang reads for nothing; no program refused or wrong.

    Measured 2026-10-07 by lane b14-ir at `dad2da47`, not repaired there: since defect 231's one exit, `slots-returns-100` (the shape rebuilt from its description) emits 7,533 lines of C and 3,133 `#line`s, 301 of them followed by another `#line`, and the compiler's own C 35,957 such of 532,475. They are the emitter's, not the IR's: `emit/inst.hero` writes the instruction's `#line` before every instruction, and a retain or a release is written under the generated file's (`reference_named`), so a release after a release gets a source `#line` no statement follows whatever line the IR gives it; defect 335's repair gave the sweep the `return`'s line and the count stayed 3,133. A probe built from a copy of the tree that skips that `#line` for `incref`, `decref` and `decref_slot` wrote 2,531 `#line`s and none dead at `slots-returns-100`, every other line identical, and 461,299 with 81 dead for its own C, 1,287,591 lines where the seed holds 1,358,691; the repair is the emit lane's file.
