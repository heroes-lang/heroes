---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **613 — the wildcard re-bound is told with a note that declares it a cell** | `_ @ f(1)`, `_ @ n + 1` and `_.x @ 2` are refused `unknown_name`, *nothing named `_` is in scope*, with panel 209's R9 note *`_ @ …` changes a cell that already exists; a new cell is declared `_ @= <value>`, and a value bound once with `_ = <value>`*: the second clause writes the wildcard cell, which declared nothing on the trunk and is refused `never_rebound` since defect 608's repair, so the note sends the reader to a refused line; beside it a rename to the nearest one-letter name in scope (`f`, `n`) is offered as a guess | `selfhost/resolve/errors.hero` `unknown_place` (the note, written for every name, the wildcard included), reached from `selfhost/resolve/writes.hero` `write_root` · **class: blocking**

    **Origin:** found by lane b20-check at 01:14 on 2026-10-11, attacking the shapes beside defect 608 (`_ @= e`) with the lane's compiler: the mutation symbol on the wildcard, measured on `_ @ f(1)`, `_ @ n + 1` and `_.x @ 2`.

    **Class: blocking**, 2026-10-11: a false message, by `.claude/rules/verification.md` § Bounded discovery's list: the note's *a new cell is declared `_ @= <value>`* names a declaration that declares nothing and is refused. Its cause is the note's, not defect 608's, so it is filed apart.
