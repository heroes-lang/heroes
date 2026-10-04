---
kind: decision
area: none
milestone: none
filed: 2026-08-03
commit: 599bfb5da3d568e58a139922b14c51cc37221c7a
github: none
---

2026-08-03 | A non-() expression in statement position is a compile error; diagnostic ships the certain fix `_ = expr` (lowers to `(void)e;`); no extern relaxation — escape valve if ever needed is a per-extern annotation at M7 | author decision, panel unanimous; Nim/Zig hard-error precedent held; implement as ⇐-check against () sharing 002's machinery | §4.14 | 003
