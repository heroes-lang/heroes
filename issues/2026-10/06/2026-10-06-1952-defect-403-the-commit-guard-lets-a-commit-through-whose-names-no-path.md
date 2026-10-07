---
kind: defect
area: process
milestone: none
filed: 2026-10-06
commit: 2232f469beedc4040b41c1d42d2b92106dff402f
github: none
---

- [ ] **403 — the commit guard lets a commit through whose `--` names no path** | lane b13-unit's path array came out empty under macOS's bash 3.2, which has no `mapfile`, so `git commit -F <file> --` took the whole index, 35 files, and missed 22 the commit meant; the lane amended it within a minute (its orphan `5550601e` is referenced nowhere). `.claude/hooks/guard_bash.py` refuses a commit with no `--` and passes one with an empty pathspec after it (the lane's report; not re-run by the coordinator) | `.claude/hooks/guard_bash.py` · CLAUDE.md § Hard stops (CL-070) · **class: improvement**

    **Origin:** lane b13-unit, 2026-10-06 (its report, *for you to decide*); filed by the coordinator at 19:52.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach, no program judged wrong; the same hazard the hard stop names, an index carrying more than the commit means, reached by an empty list. Outside the batch.

    **Measured beside it, 2026-10-06 20:08**: a conflicted merge cannot take a pathspec (git refuses a partial commit during a merge), so the guard's rule leaves it one route, and that route is unguarded: `git commit` with no `--` was refused, and `GIT_EDITOR=true git merge --continue` concluded `fae893f7`, main into round b13, with no check at all. The coordinator compared the index to `git diff --name-only HEAD...main` first, 12 files and no seed, by hand; the guard asked for nothing. A repair covers both shapes: an empty `--` and a merge concluded by `merge --continue` or a bare commit while `MERGE_HEAD` exists, the latter judged by comparing the index to the merge's own files.

    Repaired at `2232f469`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/commits.py` refuses a `--` followed by no path (but `--amend --only`), by expansions alone or by the whole tree, and a merge, cherry-pick or revert concluded by `--continue` or a bare commit while the index holds a file the operation did not bring, allowing such a bare commit otherwise; the guard reads a command behind an assignment, a wrapper or a shell's `-c`; twelve cases in `.claude/hooks/test_hooks.py` over real repositories stopped in a merge and a cherry-pick, eight red before, and 48 everyday commands read the same verdicts by both guards.
