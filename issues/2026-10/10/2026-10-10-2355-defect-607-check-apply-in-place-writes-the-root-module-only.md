---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **607 — `check --apply --in-place` writes the root module only, so a migration over a tree is one command per root** | `heroes check <root> --apply --in-place` applies the certain fixes of the root module and prints *N certain fix(es) are in another module and were not applied*; a nested module cannot be checked alone, its `use` lines resolving from its own directory (`selfhost/cli/apply_answer.hero`'s note). M-inferred-cell's two migrations, 391 cells to `=` and 1,181 files to `@=`, ran the verb over every root one at a time with a census by the compiler afterwards; the next whole-tree fix, a language change with a certain repair, pays the same. The walk is the verb's: the fixes of every module the root reaches, each written in its own file once, the root's `use` closure being what `check` already resolves | `selfhost/cli/apply_answer.hero` (the note and the root-only write), `selfhost/cli/flags.hero` (`--in-place`), `tests/harness/suite_fixes.hero` (what `--apply` owes a case) · **class: improvement**

    **Origin:** M-inferred-cell's task `…-1923-…`, found by the landing lane at 19:13 on 2026-10-10 running `./heroes check selfhost/main.hero --apply` and reading its note; re-filed at the milestone's close, 23:55, since a milestone's open task must name an open milestone and this one is nobody's.

    **Class: improvement**, 2026-10-10: nothing is wrong at exit 0 and no message is false; a migration is possible today, one root at a time, and this would make it one command.
