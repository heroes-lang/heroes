---
kind: decision
area: process
milestone: none
filed: 2026-08-12
commit: e6c22a29c8f2af3c208982f1095778283f38162c
github: none
---

2026-08-12 | **The C symbol component is the module's whole path, sanitised and concatenated — never its last part.** Measured over the port's 169 real paths: last-part components collide **22-way on `mod`** alone, whole-path concatenation collides **zero** times. This holds whether or not paths ever enter the language, and it is the shape `module_names_collide` must compare | the two namespaces are not the same namespace: what the author types before the dot and what reaches the linker have different collision sets, and one check was written over the wrong one | §3.1, CLAUDE.md §7 | 032 |
