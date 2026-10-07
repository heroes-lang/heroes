---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: 4425b5440b46d6c6ecf09813bef20fe4c3006d88
github: none
---

- [ ] **334 — the commit guard runs `heroes fmt` over every staged `.hero`, so a golden case holding a parse error on purpose is refused once it is staged before the commit's own command** | concluding batch 10's merge of lane b10-cli with `git commit -- <paths>`: the guard ran `fmt` over the eight staged cases of defects 324 and 325 (`tests/golden/check/fixedbugs-324-*`, `fixedbugs-325-*`), each holding a lexer error on purpose, and refused the commit with *does not parse*; the lane had committed the same cases because its `git add` and `git commit` shared one command line, and the guard reads the index before the command runs, so it saw nothing staged (the coordinator, 2026-10-04, concluded the merge by `git merge --continue`) | `.claude/hooks/guard_bash.py` (`staged_offences`, `fmt` over `git diff --cached` at the commit's start) · CL-079's layer 1 · **class: improvement**

    **Origin:** the coordinator, 2026-10-04, at batch 10's merge of lane b10-cli.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument that refuses a correct commit on one path and sees nothing on another; hardening of the guard, which should judge a golden case by its marks rather than by `fmt`.

    Repaired at `4425b544`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. A staged golden case whose marks claim diagnostics is judged by them: the commit's cases of the `annotations` suite's `DIRECTORIES` are asked of that suite in one narrowed run bounded at 45 s (`.claude/hooks/marks.py`, which the write hook asks through too), its run roots are left to the gate, and a marked `run/` program keeps `fmt`'s verdict; eleven cases in `.claude/hooks/test_hooks.py`, six red before, and defects 324 and 325's eight cases staged in a scratch repository, all refused *does not parse* by the base guard, were judged once and the one mark planted wrong found, at 80.1 billion instructions and 13.0 s of user time a run.
