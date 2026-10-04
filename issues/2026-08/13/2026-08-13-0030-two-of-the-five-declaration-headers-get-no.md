---
kind: learn
area: none
milestone: M-syntax-tree
filed: 2026-08-04
commit: ee88e7dc42d807028fee3d9a39e51cba31d81e6e
github: none
---

- [x] The record | M2.1 | Two of the five declaration headers get NO terminator from the lexer. Which two, and why does the body parser skip terminators instead of requiring one? | crates/heroes/src/syntax/decl.rs (module doc), lexer/layout.rs is_line_ender | panel 007's rule, seen from the other side of the fence
