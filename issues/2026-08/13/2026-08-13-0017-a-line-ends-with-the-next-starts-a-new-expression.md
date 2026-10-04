---
kind: learn
area: design
milestone: M-token-stream
filed: 2026-08-04
commit: 200b6fe21d9874d14543bf8b7506fa6a12b2582c
github: none
---

- [x] The record | M1.1 | A line ends with `+`; the next starts a new expression. Does the lexer emit a terminator after the `+` — yes or no? And after a `)`? | crates/heroes/src/lexer/layout.rs (is_line_ender, design.md §4.15) | this one rule is how Heroes lives without semicolons
