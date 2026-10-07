---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: ecb484feecfe76c895768b64c2404a01a96cbc69
github: none
---

- [ ] **332 — a compiler path naming a directory is accepted, and then every case fails as a program that could not be started** | `-- ./somedir` or `HEROES_COMPILER=./somedir`: the harness accepts the path, and each case of every suite then fails as a program that could not be started, one failure per case where one refusal at the start would say it | `tests/harness/shell.hero` (`some_compiler`) · defect 316 · **class: improvement**

    **Origin:** lane b10-harness, 2026-10-04, measured on its worktree at `bd1f168c` (its final reply's *Found beside*), beside 316.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a loud but scattered failure; hardening of the net's start.

    Repaired at `ecb484fe`, 2026-10-07 (lane b14-harness-a), gated by its cases and the net's own tests; the net is owed at the batch's close. `shell.unstartable` asks what a compiler path is (nothing, an empty word, a link to nothing, a directory) and has the machine start it with `--version`, naming the operating system's reason where it will not; the net's start asks it of `-- <compiler>` (exit 2, one message), `some_compiler` of `HEROES_COMPILER` and of `./heroes`, the first place it looks. Measured on the base: `-- <dir> ir` 26 failures *could not be started*, `HEROES_COMPILER=<dir>` about 4,000 over 23 suites; after, a folder, a mode-644 file, a link to nothing, `""` and a missing path each one message at exit 2 by both routes; the net's own tests 291, all passed.
