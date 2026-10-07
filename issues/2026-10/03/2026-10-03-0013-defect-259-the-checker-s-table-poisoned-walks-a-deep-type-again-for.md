---
kind: defect
area: check
milestone: none
filed: 2026-10-03
commit: ce369512151c18bae33ee6c0f0f4b0c99eedb97c
github: none
---

- [ ] **259 — the checker's `table.poisoned` walks a deep type again for every expression that holds it** | 2,005,125 calls on `index-chain-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/check/table.hero:429` (`poisoned`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the type's depth times the expressions; no program refused or wrong.

    Repaired at `ce369512`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `intern` writes each type's poison beside it once, from what it holds, and `poisoned` reads that entry: on an index chain 2,000 deep, `check --brief` ran `poisoned` 2,005,128 times before (2,005,125 on the emit lane's tip `96473596`, re-run) and 2,089 after, and retired 2,252M instructions before and 1,203M after, every output byte-identical.
