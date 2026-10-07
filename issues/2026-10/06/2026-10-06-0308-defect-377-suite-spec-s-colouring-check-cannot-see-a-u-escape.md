---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: 3c687c3aff0a548d2a69b79db00015085ed57776
github: none
---

- [ ] **377 — suite_spec's colouring check cannot see a `\u{...}` escape** | lane b12-str192, landing panel 192's R5, tested the editor grammar's pattern for `\u{...}` with Python's `re` alone, since `tests/harness/suite_spec.hero`'s colouring check reads no escape (its report, 2026-10-06) | `tests/harness/suite_spec.hero` (the colouring check) · `editors/vscode/syntaxes/heroes.tmLanguage.json` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): coverage, no program moves.

    Repaired at `3c687c3a`, 2026-10-07 (lane b14-harness-b), gated by its cases and the net's own tests; the net is owed at the batch's close. `tests/harness/textmate.hero` runs the grammar's rules for a string, an `f` literal and a character literal as an editor does, and `suite_spec`'s `escapes_painted` holds them to the lexer on the seven escapes § 2 names and six other spellings of its `\u{1b}`, 39 `heroes lex` runs: on a copy of the tree, three breaks of the `\u{…}` pattern each read spec 21 passed, 0 failed on the base and 22 and 1 with this check, which reads 23 and 0 on the grammar as it is.
