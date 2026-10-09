---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **529 — the commit guard does not watch the path-limited git commands that discard a peer's staged work** | `git restore -S -W -- <paths>`, `git checkout HEAD -- <paths>` and `git checkout <commit> -- <paths>` throw away the staged change of every path named, whoever staged it, and `git rm -f <paths>` the file; defect 514's repair watches the whole-index discards (`merge --abort` and its kin) and none of these; the unstaged-only `git restore -- <paths>` and `git checkout -- <paths>` are the hard stops' *destructive operations are asked for*, which no hook performs (lane b16-tools, probe7, git 2.56.0) | `.claude/hooks/discards.py`, `.claude/hooks/guard_bash.py` · defect 514 · **class: adjacent**

    **Origin:** filed by the coordinator at 11:10 on 2026-10-09 from lane b16-tools's notes (`.claude/worktrees/scratch-b15/tools/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a peer's work in the shared checkout the hard stops name.
