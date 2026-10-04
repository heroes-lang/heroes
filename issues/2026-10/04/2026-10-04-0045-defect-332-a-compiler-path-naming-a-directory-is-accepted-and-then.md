---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **332 — a compiler path naming a directory is accepted, and then every case fails as a program that could not be started** | `-- ./somedir` or `HEROES_COMPILER=./somedir`: the harness accepts the path, and each case of every suite then fails as a program that could not be started, one failure per case where one refusal at the start would say it | `tests/harness/shell.hero` (`some_compiler`) · defect 316 · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*), beside 316.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a loud but scattered failure; hardening of the net's start.
