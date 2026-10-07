---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: 2a2047748c95a9103f03e76736bb51d97afe6c1b
github: none
---

- [ ] **258 — `cursor.is_invisible` costs the square of the block depth** | 4,014,367 calls on `match-nested-1000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/cursor.hero:194` (`is_invisible`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square; no program refused or wrong.

    Repaired at `2a204774` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The last walk back is kept on the cursor (`stream_tables`) and a walk that meets where it began goes on from where it ended, the parser's writes to its stream going through `stream_tables.relaid`: `is_invisible` asked 3,362, 6,362 and 12,362 times on `match` nested 250, 500 and 1,000 deep, where it was asked 253,896, 1,007,396 and 4,014,396 (reproduced on `dad2da47`), and `parse selfhost/main.hero` 0.16% dearer.
