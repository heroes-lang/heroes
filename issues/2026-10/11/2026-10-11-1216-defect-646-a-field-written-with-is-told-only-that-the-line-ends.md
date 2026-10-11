---
kind: defect
area: compiler
milestone: none
filed: 2026-10-11
commit: a7ec4dd87f69cd8e003c7402916615ef66eb7707
github: none
---

- [ ] **646 — a field written with `=` is told only that the line ends** | `function move(@p: P)` over `p.x = p.x + 1`: `check` tells `expected_end_of_line` at the `=`, *expected the end of the line, found `=` — one statement per line, no semicolons*, with no hint that `p.x @ p.x + 1` was meant and no fix, where the same slip on a bare name, `rows = rows.push(1)` over an `@` parameter, is told as a shadow offering `@` since defect 662, and on a field with the cell's symbol, `p.x @= 1`, as `declared_place` with the fix writing `@`. The same for an element, `xs[0] = 5`, and a nested field, `a.b.c = 1`: the statement is read as the expression `p.x`, and the `=` after it is only a token past the line's end | `selfhost/cell_errors.hero` `written` (what follows an expression at a statement's start: `@` and `@=` told, `=` not), `selfhost/grammar_expr.hero` `expression_or_mutation`, `selfhost/parse/statement_end.hero` `close` (the generic message) · **class: adjacent**

    **Origin:** found 2026-10-11 by lane b22-662, measuring the shapes beside defect 662 (a field of an `@` parameter written with `=`), and filed by the coordinator's instruction as 646.

    **Class: adjacent**, 2026-10-11: a true message less exact than it could be, the slip it names being one whose repair is a single symbol.

    Repaired at `a7ec4dd87f69cd8e003c7402916615ef66eb7707`, 2026-10-11 (lane b22-662), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `cell_errors.written` reads `=` after a field or an element place as it reads `@=`: one `declared_place` at the `=`, *`p.x = …` would bind a name, and a field or an element is no name — it is written with `@`: `p.x @ …`*, the line read as the write, its fix a `guess` writing `@`. The parser cannot see whether the root is mutable, so a parameter with no `@` or a `=` binding at the root is told the same, and the resolver's `not_mutable` follows once the `@` is written; no fix is withheld there, which is the one way this falls short of the coordinator's brief (a fix only where the root is mutable). A left side that is no place, `f(1) = 2`, keeps `expected_end_of_line`. Shapes measured: a field of an `@` parameter, an element of a cell, a nested field, an element's field, a root not mutable, each one `declared_place` where the trunk told `expected_end_of_line`. Four `tests/golden/check/fixedbugs-646-*` cases, and defect 662's field case moved to `declared_place`, corrected beneath its header; check 699/0 whole, check, fixes and annotations on fixedbugs-646 4/0 and on fixedbugs-662 5/0, full 38/0, permissive 21/0, layout on `cell_errors.hero` 4/0, own tests 1617 passed.
