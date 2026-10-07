---
kind: defect
area: process
milestone: none
filed: 2026-10-04
commit: 2990e653df33ba43d3db92742e598d814026baa4
github: none
---

- [ ] **287 — the guard's staged-file check reads the session directory's index, so a lane's commit is never checked** | `staged_hero_files(cwd)` runs `git diff --cached` in the session's directory (`.claude/hooks/guard_bash.py:247`), so a commit made in a lane's worktree is judged by the trunk's index: lane b9-harness's `a3fb46d5` staged three cases `heroes fmt` refuses and the guard accepted it (measured by the lane, 2026-10-04); in the trunk the same check refuses a golden case whose intended diagnostic makes `fmt` refuse it (defect 272's shape) | `.claude/hooks/guard_bash.py:247` to `:290` · defects 254 and 272, the layer-0 hook's two · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, measured).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach; no program moves.

    Repaired at `2990e653`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. The staged-file check, now `.claude/hooks/staged.py`, reads the index of the directory the commit runs in, `git -C` or the command's own `cd`, its paths from that worktree's root, judged by that tree's compiler and told as that compiler's age where it is older than its tree; six cases in `.claude/hooks/test_hooks.py` over real repositories and a worktree, five red before, and a scratch lane's staged module `fmt` refuses passed the base guard from the trunk and is refused by the repaired one.
