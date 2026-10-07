---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: 44ca5997123181759968f13ac128a32cacf29674
github: none
---

- [ ] **264 — `inventory.table()` builds the table of built-ins again for every lookup** | 31,234,497 copies on `method-chain-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/inventory.hero:22` (`table`) and its callers · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost per lookup; no program refused or wrong.

    Repaired at `44ca5997`, 2026-10-07 (lane b14-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The names are the constant `NAMES`, which `index_of` and `name_of` read, the checker's four lookups read `name_of`, and `labelled_of` builds only the entry it answers with: on the compiler's own check the tables built fell from 137,680 to 0 and its instructions retired from 65.9G to 63.6G; `table()` keeps 18,030 calls on `build --emit-c` of a chain of 2,000 built-in calls, every one in ir/ and emit/, which `name_of` can take. The 31,234,497 copies above did not reproduce on `method-chain-2000` in two forms, on the base or on `96473596` re-run (4,281 and 36,277 tables).
