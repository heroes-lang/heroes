---
kind: learn
area: none
milestone: M-syntax-tree
filed: 2026-08-04
commit: ee88e7dc42d807028fee3d9a39e51cba31d81e6e
github: none
---

- [x] The record | M2.2 | Three line shapes start with a name: `x = 5`, `v: int @ 0`, `v @ v + 1`. How many tokens of lookahead tell them apart, and which token is it in each case? | crates/heroes/src/syntax/stmt.rs (module doc, statement) | §4.4 designed the surface so this is decidable without scanning the block
