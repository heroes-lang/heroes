---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **257 — the parser's `pairing.is_closer` costs the square of the bracket depth** | 40,040,825 calls on `record-literal-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/parse/pairing.hero:54` (`is_closer`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square; no program refused or wrong.
