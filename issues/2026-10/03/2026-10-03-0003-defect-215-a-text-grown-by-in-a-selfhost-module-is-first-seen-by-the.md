---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **215 — a text grown by `+` in a selfhost module is first seen by the whole `layout` at a gate: the write-time hook does not ask it, and `layout` narrowed to the file cannot** | lane win214's `b8ad7f9e` wrote `beside @ beside + ch` in a loop of `selfhost/cli/compiling.hero`'s test; `.claude/hooks/fmt_check.py` passed it (it asks parse, canonical form, the compiler's check and the line ceiling), and the lane's gate read `layout/concat` red at 11:06, four sites; `tests/harness/suite_layout.hero` asks `appends`, `concat` and `budget` only when `only == ""`, so a narrowed `layout` never asks them | `.claude/hooks/fmt_check.py`, `tests/harness/suite_layout.hero` (`GROWTH_ALLOWED`) · defect 209 · **class: improvement**

    **Origin:** the coordinator, 2026-10-03, at lane win214's gate: one run of 25 suites stopped at 20 to repair it, then run again whole.

    **Class: improvement**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an instrument's coverage. § A suite is the last judge asks that what a hook can see on the touched file never wait for a suite, and the growth sites of one file against `GROWTH_ALLOWED`'s entries for that file are such a thing.
