---
kind: feature
area: parse
milestone: M-inferred-cell
filed: 2026-10-10
commit: 65cd1145d7aadf82e24d5910d12e155a15e0c97a
github: none
---

- [x] **M-inferred-cell** | the `@=` token declares a cell, `v: T @= e` with the type kept (panel 209's R10, the landing's second commit): `selfhost/punctuation.hero`'s two-byte match, `token.hero`'s `at_eq` in its tables and the seven enumerated `TokenKind` tables elsewhere, `grammar_expr.hero`'s `annotated`, `print/fmt.hero` and `print/bodies.hero`, `parse/annotation.hero`, `at_prefix.lent_with_eq` for `(@=x` with a certain fix `write @x`; `v: T @ e` refused by `expected_binding_symbol` with a certain fix writing `@=`; the three ceilings' seams (one leaf under `selfhost/check/` for the `bind`/`declare` arms' shared lines, `cell_stmt` and `bind_stmt` as one function, `lent_with_eq` outside `selfhost/parse/` or a DECIDED row); the formatter's probe reader, `heroes mutate`, the TextMate grammar and `site/src/lib/highlight.ts` taught the token; and the whole tree rewritten in the same commit, `canonical` being red between the token and the rewrite | `docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md` § The landing; the engineer's prototype, `.claude/worktrees/scratch-b15/209-compiler-engineer/209-notes/diff-step1.txt` (ignored by git); `.claude/rules/diagnostics-and-goldens.md` § A new surface form

    **Origin:** panel 209's synthesis, 2026-10-10, ratified 17:57; filed at the milestone's opening, 19:20.

    **Closed** at step 2, `65cd1145`, 2026-10-10: the token, the printers, the editors' grammar, the library source and 1,181 files rewritten through `check --apply`; the spec's lines with the type kept at 10,029 real; the census clean.
