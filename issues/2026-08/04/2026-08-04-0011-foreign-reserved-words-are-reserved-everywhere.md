---
kind: decision
area: none
milestone: none
filed: 2026-08-04
commit: 39e4ebe6e8da7aa67aa9783da2ac22b2d34dba82
github: none
---

2026-08-04 | Foreign reserved words are reserved EVERYWHERE, including as user identifiers: the registry's words cannot name a variant case, a field or a local (found by parsing the appendix, whose `Expr.var` case did not lex; renamed `.variable`). The price is now on the record — `var case union use include class try const` are unusable as names — and queued for a panel if it ever bites a real program | consequence of panel 013's "one word, one meaning, everywhere", applied to the other side of the boundary; the alternative is contextual lexing, which panel 013 rejected on four judges' evidence | §4.17, §4.2 | 013
