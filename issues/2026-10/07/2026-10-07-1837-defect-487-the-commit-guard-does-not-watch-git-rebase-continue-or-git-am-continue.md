---
kind: defect
area: process
milestone: none
filed: 2026-10-07
commit: 699241f6135090962b134c96144444042ba5e1b1
github: none
---

- [ ] **487 — the commit guard does not watch `git rebase --continue` or `git am --continue`** | both commit the whole index; defect 403's guard covers merge, cherry-pick and revert only (lane b14-hooks) | `.claude/hooks/commits.py` (batch 14's round, unmerged on 2026-10-07) · defect 403 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach, the hazard the hard stop names.

    Repaired at `699241f6`, 2026-10-08 (lane b15-hooks), gated by its cases and the hooks' own tests; the net is owed at the batch's close. The commit guard reads a rebase's and an am's `--continue` (an am's `--resolved` and `-r`, and every abbreviation git takes, `git merge --cont` included) and refuses it when the index holds a file the stop did not bring: a rebase's pick, or the merge it redoes, read from `REBASE_HEAD` and `MERGE_HEAD`, an am's patch by `git apply --numstat` both ways, a root commit picked by `git diff-tree --root`; a bare commit while a rebase or an am stands keeps its refusal, git taking a pathspec then (measured, git 2.56.0). 13 tests in `test_hooks.py`'s `Sequences`, 9 red on the base; the hooks' own tests 76, OK.
