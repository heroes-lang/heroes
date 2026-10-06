---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **377 — suite_spec's colouring check cannot see a `\u{...}` escape** | lane b12-str192, landing panel 192's R5, tested the editor grammar's pattern for `\u{...}` with Python's `re` alone, since `tests/harness/suite_spec.hero`'s colouring check reads no escape (its report, 2026-10-06) | `tests/harness/suite_spec.hero` (the colouring check) · `editors/vscode/syntaxes/heroes.tmLanguage.json` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): coverage, no program moves.
