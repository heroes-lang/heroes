---
kind: defect
area: check
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **259 — the checker's `table.poisoned` walks a deep type again for every expression that holds it** | 2,005,125 calls on `index-chain-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/check/table.hero:429` (`poisoned`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the type's depth times the expressions; no program refused or wrong.
