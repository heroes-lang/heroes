---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 09a6a5a1976ae364f6b48f1f1cd399ffafe53c79
github: none
---

- [x] **514 — the commit guard does not watch the git commands that discard staged work** | `git merge --abort`, `git rebase --abort`, `git rebase --skip`, `git cherry-pick --abort` and `--skip` delete a peer's staged new file and revert its staged change at exit 0, and `git rebase --autostash` runs a stash the hard stop forbids; reproducer: stop a merge on a conflict, `printf c > c.txt; git add c.txt`, `git merge --abort`, and `c.txt` is gone (lane b15-hooks, git 2.56.0) | `.claude/hooks/guard_bash.py`, `.claude/hooks/commits.py` · defects 403 and 487 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-hooks's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument's reach over a peer's work in the shared checkout the hard stops name.

    Repaired at `cdd3b65e`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/discards.py`: an abort or a skip of a merge, a pick, a revert, a rebase or an am, and `reset --merge` and `--hard`, are refused where the tree holds a change they would throw away in a file the operation standing did not bring, a rebase's and `reset --hard` counting an unstaged change too, as measured; a rebase, a merge or a pull begun with an autostash (flag, `-c`, environment or configuration) where the tree holds a tracked change. 16 cases, 14 red on the base; the hooks' own tests 102 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.

    Repaired at `09a6a5a1` too, 2026-10-09, the same cause found by the lane's probe beside it: `git checkout -f` of the whole tree and `git switch -f` and `--discard-changes` throw away staged and unstaged changes as `reset --hard` does, and are read as it is; a checkout naming a path is left. The class 17 cases, the hooks' own tests 111 run and the same 2 failed.

## The repair

Repaired at `cdd3b65e`, 2026-10-09 (lane b16-tools, batch 16), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `.claude/hooks/discards.py`: an abort or a skip of a merge, a pick, a revert, a rebase or an am, and `reset --merge` and `--hard`, are refused where the tree holds a change they would throw away in a file the operation standing did not bring, a rebase's and `reset --hard` counting an unstaged change too, as measured; a rebase, a merge or a pull begun with an autostash (flag, `-c`, environment or configuration) where the tree holds a tracked change. 16 cases, 14 red on the base; the hooks' own tests 102 run and 2 failed, the two `Place` tests the base fails the same way with its temporary folder inside the repository.

Repaired at `09a6a5a1` too, 2026-10-09, the same cause found by the lane's probe beside it: `git checkout -f` of the whole tree and `git switch -f` and `--discard-changes` throw away staged and unstaged changes as `reset --hard` does, and are read as it is; a checkout naming a path is left. The class 17 cases, the hooks' own tests 111 run and the same 2 failed.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
