---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 5ddf3f4d320628bc6f0992c73bc08690eb02d15f
github: none
---

- [ ] **257 — the parser's `pairing.is_closer` costs the square of the bracket depth** | 40,040,825 calls on `record-literal-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/parse/pairing.hero:54` (`is_closer`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square; no program refused or wrong.

    Repaired at `5ddf3f4d` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On `dad2da47` the item's own shape is linear since defect 344 (`b93bf7f8`): a record literal 2,000 deep asks `token.is_closer` 8,984 times. The square was left where no walk paired the cursor, a view of braces and a plain literal's hole, and each is paired as it is made now: in braces 2,000 deep 16,016,994 calls to 16,998 and `check --brief` 17.39 billion instructions retired to 0.98; in a hole 1,000 deep 1,003,973 calls to 1,974.
