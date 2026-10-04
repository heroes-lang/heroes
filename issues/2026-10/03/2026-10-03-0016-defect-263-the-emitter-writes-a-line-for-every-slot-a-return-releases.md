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
