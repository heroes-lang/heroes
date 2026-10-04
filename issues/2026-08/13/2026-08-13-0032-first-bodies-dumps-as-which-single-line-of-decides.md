---
kind: learn
area: none
milestone: M-syntax-tree
filed: 2026-08-04
commit: ee88e7dc42d807028fee3d9a39e51cba31d81e6e
github: none
---

- [x] The record | M2.2 | First bodies. `x = 2 + 3 * 4` dumps as `(2 + (3 * 4))`. Which single line of `expr.rs` decides that, and what would `x = 2 * 3 + 4` dump as? | crates/heroes/src/syntax/expr.rs (binary_op), tests/bodies.rs | the dump is fully parenthesised so that it *proves* the precedence table
