---
kind: defect
area: compiler
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **646 — a field written with `=` is told only that the line ends** | `function move(@p: P)` over `p.x = p.x + 1`: `check` tells `expected_end_of_line` at the `=`, *expected the end of the line, found `=` — one statement per line, no semicolons*, with no hint that `p.x @ p.x + 1` was meant and no fix, where the same slip on a bare name, `rows = rows.push(1)` over an `@` parameter, is told as a shadow offering `@` since defect 662, and on a field with the cell's symbol, `p.x @= 1`, as `declared_place` with the fix writing `@`. The same for an element, `xs[0] = 5`, and a nested field, `a.b.c = 1`: the statement is read as the expression `p.x`, and the `=` after it is only a token past the line's end | `selfhost/cell_errors.hero` `written` (what follows an expression at a statement's start: `@` and `@=` told, `=` not), `selfhost/grammar_expr.hero` `expression_or_mutation`, `selfhost/parse/statement_end.hero` `close` (the generic message) · **class: adjacent**

    **Origin:** found 2026-10-11 by lane b22-662, measuring the shapes beside defect 662 (a field of an `@` parameter written with `=`), and filed by the coordinator's instruction as 646.

    **Class: adjacent**, 2026-10-11: a true message less exact than it could be, the slip it names being one whose repair is a single symbol.
