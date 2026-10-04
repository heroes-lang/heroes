---
kind: defect
area: process
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **287 — the guard's staged-file check reads the session directory's index, so a lane's commit is never checked** | `staged_hero_files(cwd)` runs `git diff --cached` in the session's directory (`.claude/hooks/guard_bash.py:247`), so a commit made in a lane's worktree is judged by the trunk's index: lane b9-harness's `a3fb46d5` staged three cases `heroes fmt` refuses and the guard accepted it (measured by the lane, 2026-10-04); in the trunk the same check refuses a golden case whose intended diagnostic makes `fmt` refuse it (defect 272's shape) | `.claude/hooks/guard_bash.py:247` to `:290` · defects 254 and 272, the layer-0 hook's two · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, measured).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.
