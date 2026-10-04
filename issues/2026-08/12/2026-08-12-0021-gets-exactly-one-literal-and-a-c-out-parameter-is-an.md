---
kind: decision
area: process
milestone: none
filed: 2026-08-12
commit: 908388a566d0a667b641cf4178d4008dfa2df3e2
github: none
---

2026-08-12 | **`ptr` gets exactly one literal, `null`, and a C out-parameter is an `@` parameter.** The llm-ergonomist could not write the milestone's own acceptance program: `sqlite3_open(path, &db)` needs a `ptr` value and the language produces none — no null, no address-of, no allocation, and every binding is initialised. CLAUDE.md §7 already makes an `@` parameter a pointer parameter, so the out-parameter half costs nothing new | an acceptance test that cannot be written is a milestone that cannot be finished, and the gap was invisible to everyone who already knew the compiler | §4.19, §4.8 | 036 |
