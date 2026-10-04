---
kind: decision
area: none
milestone: none
filed: 2026-08-04
commit: 908571490f7cdca94152d1cf139e51d2eab0735c
github: none
---

2026-08-04 | Escape sequences: five, split by context (Go's rule) — strings `\n \t \\ \"`, char literals `\n \t \\ \'`; backslash reserved, any other escape a compile error with a certain fix; `\r` cut; set frozen (`\0`/`\xNN`/`\u{}` reconvene the panel — ffi holds a veto: interior NUL voids §4.20's free `.cstr()`); char contract restated (one ASCII byte OR one escape) | panel 008, author ratified: without `\"` the language was INCOMPLETE (a quote inside a string was unwritable) and `"a\nb"` compiled printing four characters — a silent wrong-output trap, the anti-thesis; printf's `"%d\n"` and sqlite3_bind_text data were unexpressible (ffi compiled the proof) | §4.3, §4.15 | 008
