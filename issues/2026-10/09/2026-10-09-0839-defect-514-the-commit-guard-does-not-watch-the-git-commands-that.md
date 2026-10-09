---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **514 — the commit guard does not watch the git commands that discard staged work** | `git merge --abort`, `git rebase --abort`, `git rebase --skip`, `git cherry-pick --abort` and `--skip` delete a peer's staged new file and revert its staged change at exit 0, and `git rebase --autostash` runs a stash the hard stop forbids; reproducer: stop a merge on a conflict, `printf c > c.txt; git add c.txt`, `git merge --abort`, and `c.txt` is gone (lane b15-hooks, git 2.56.0) | `.claude/hooks/guard_bash.py`, `.claude/hooks/commits.py` · defects 403 and 487 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a peer's work in the shared checkout the hard stops name.
