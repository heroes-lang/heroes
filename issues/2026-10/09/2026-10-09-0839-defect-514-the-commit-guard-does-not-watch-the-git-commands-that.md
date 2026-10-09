---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: cdd3b65e2918cce5b3b65346426efbc29afdbf92
github: none
---

- [ ] **514 — the commit guard does not watch the git commands that discard staged work** | `git merge --abort`, `git rebase --abort`, `git rebase --skip`, `git cherry-pick --abort` and `--skip` delete a peer's staged new file and revert its staged change at exit 0, and `git rebase --autostash` runs a stash the hard stop forbids; reproducer: stop a merge on a conflict, `printf c > c.txt; git add c.txt`, `git merge --abort`, and `c.txt` is gone (lane b15-hooks, git 2.56.0) | `.claude/hooks/guard_bash.py`, `.claude/hooks/commits.py` · defects 403 and 487 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a peer's work in the shared checkout the hard stops name.

    Repaired at `cdd3b65e`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/discards.py`: an abort or a skip of a merge, a pick, a revert, a rebase or an am, and `reset --merge` and `--hard`, are refused where the tree holds a change they would throw away in a file the operation standing did not bring, a rebase's and `reset --hard` counting an unstaged change too, as measured; a rebase, a merge or a pull begun with an autostash (flag, `-c`, environment or configuration) where the tree holds a tracked change. 16 cases, 14 red on the base; the hooks' own tests 102 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.
