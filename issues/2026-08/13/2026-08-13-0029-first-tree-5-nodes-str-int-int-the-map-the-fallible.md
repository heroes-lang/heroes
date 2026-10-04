---
kind: learn
area: none
milestone: M-syntax-tree
filed: 2026-08-04
commit: ee88e7dc42d807028fee3d9a39e51cba31d81e6e
github: none
---

- [x] The record | M2.1 | First tree. `{str: [int]}?` — 5 nodes (str · int · [int] · the map · the fallible wrapper); now pinned by `the_arena_holds_one_node_per_written_constructor` | crates/heroes/src/syntax/types.rs, tests/types.rs | the arena is why a recursive type needs no pointers, which is why it ports to Heroes
