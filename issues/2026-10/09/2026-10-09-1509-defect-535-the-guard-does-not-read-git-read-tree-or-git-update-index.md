---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **535 — the guard does not read `git read-tree` or `git update-index --force-remove`** | `git read-tree HEAD` loses the staged version of a path changed again after staging, and `git update-index --force-remove` acts as `git rm --cached -f`; defect 529's repair reads neither, both plumbing nobody here types by hand (lane b16-misc) | `.claude/hooks/overwrites.py` · defect 529 · **class: improvement**

    **Origin:** filed by the coordinator at 15:09 on 2026-10-09 from lane b16-misc's final report (its notes `.claude/worktrees/scratch-b15/misc/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): hardening against commands no session here runs.
