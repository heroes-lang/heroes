---
kind: learn
area: records
milestone: M-syntax-tree
filed: 2026-08-04
commit: ee88e7dc42d807028fee3d9a39e51cba31d81e6e
github: none
---

- [x] The record | M2.3 | `x = 2 + (3 * 4)` formats to `x = 2 + 3 * 4`, but `x = a - (b - c)` keeps its parentheses → distilled into docs/glossary/002-binding-power-and-position.md (power says how tight, position says which side) | crates/heroes/src/printer/fmt_expr.rs (wrapped) | associativity is the half of the precedence table nobody writes down
