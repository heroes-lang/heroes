---
kind: decision
area: golden
milestone: none
filed: 2026-08-04
commit: 04badae322181b73774428f10376bed4554fe6be
github: none
---

2026-08-04 | `tests/golden/ir/` inherits `check/`'s `UPDATE_GOLDEN` ban | the compiler-engineer's condition for accepting one lowering pass with no desugared tree: with no second tree to inspect, these expectations are the only evidence Part 5's sugar table was honoured, and an expectation that can be regenerated mechanically is evidence of nothing | CLAUDE.md §9 | 019 |
