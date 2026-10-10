---
kind: defect
area: resolve
milestone: none
filed: 2026-10-11
commit: 71f0820684c9da5e20dcaf3b3d4e42147e42b6e8
github: none
---

- [ ] **613 — the wildcard re-bound is told with a note that declares it a cell** | `_ @ f(1)`, `_ @ n + 1` and `_.x @ 2` are refused `unknown_name`, *nothing named `_` is in scope*, with panel 209's R9 note *`_ @ …` changes a cell that already exists; a new cell is declared `_ @= <value>`, and a value bound once with `_ = <value>`*: the second clause writes the wildcard cell, which declared nothing on the trunk and is refused `never_rebound` since defect 608's repair, so the note sends the reader to a refused line; beside it a rename to the nearest one-letter name in scope (`f`, `n`) is offered as a guess | `selfhost/resolve/errors.hero` `unknown_place` (the note, written for every name, the wildcard included), reached from `selfhost/resolve/writes.hero` `write_root` · **class: blocking**

    **Origin:** found by lane b20-check at 01:14 on 2026-10-11, attacking the shapes beside defect 608 (`_ @= e`) with the lane's compiler: the mutation symbol on the wildcard, measured on `_ @ f(1)`, `_ @ n + 1` and `_.x @ 2`.

    **Class: blocking**, 2026-10-11: a false message, by `.claude/rules/verification.md` § Bounded discovery's list: the note's *a new cell is declared `_ @= <value>`* names a declaration that declares nothing and is refused. Its cause is the note's, not defect 608's, so it is filed apart.

    Repaired at `71f0820684c9da5e20dcaf3b3d4e42147e42b6e8`, 2026-10-11 (lane b20-check), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `resolve/writes.write_root` tells the wildcard apart before any near name is sought (`errors.wildcard_place`, code `unknown_name`): *`_` binds nothing, so there is no cell for `@` to re-bind there — a value dropped on purpose is written `_ = …`*, no note and no fix. Shapes measured: `_ @ f(1)`, `_ @ n + 1`, `_.x @ 2`, `_[0] @ 3`, each told so once; and a name one letter from `_` (`k = 4` beside `_ @ 5`), which the trunk spared its own `unused_binding` because it had been offered as the rename, now told at once. A read of `_` (`print(_)`, *nothing named `_` is in scope — did you mean `n`?*) and a lend (`bump(@_)`) are the read side's message, unchanged and not this item's. Three `tests/golden/check/fixedbugs-613-*` cases; check, fixes and annotations 3/0 narrowed, check 681/0, fixes 978/0 and annotations 1035/0 whole, own tests 1597 passed; instructions retired within run-to-run spread (the commit body's numbers).
