---
kind: defect
area: resolve
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **260 — `resolve/writes.overlaps` compares a function's writes pairwise** | 319,600 calls on `many-params-800` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/resolve/writes.hero:237` (`overlaps`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the parameters' square; no program refused or wrong.
