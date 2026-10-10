---
kind: defect
area: process
milestone: none
filed: 2026-10-09
commit: 59e6ca86a326534180974b7b12814f41864ffc0d
github: none
---

- [x] **535 — the guard does not read `git read-tree` or `git update-index --force-remove`** | `git read-tree HEAD` loses the staged version of a path changed again after staging, and `git update-index --force-remove` acts as `git rm --cached -f`; defect 529's repair reads neither, both plumbing nobody here types by hand (lane b16-misc) | `.claude/hooks/overwrites.py` · defect 529 · **class: improvement**

    **Origin:** filed by the coordinator at 15:09 on 2026-10-09 from lane b16-misc's final report (its notes `.claude/worktrees/scratch-b15/misc/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): hardening against commands no session here runs.

    Repaired at `59e6ca86`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `overwrites.py` reads `git read-tree` (one tree or several, `--reset`, `--empty` and no tree at all over the index; `-m` of one tree over a path the disk no longer holds; `-m -u` over the working tree too; `--reset -u` of one tree a hard reset, read in `discards.py`) and `git update-index` in order (`--force-remove` and `--remove` binding the paths after them, `--cacheinfo` and `--index-info` writing an entry over the staged one, `--stdin` every path), each refused where a staged version would be nowhere, as measured on git 2.56.0; two or three trees, `--prefix`, `--index-output`, `-n` and `--again` lose nothing and pass. 5 tests, 4 red on the base; the hooks' own tests 140, all passed.

## The repair

Repaired at `59e6ca86`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the hooks' own tests; the net is owed at the batch's close. `overwrites.py` reads `git read-tree` (one tree or several, `--reset`, `--empty` and no tree at all over the index; `-m` of one tree over a path the disk no longer holds; `-m -u` over the working tree too; `--reset -u` of one tree a hard reset, read in `discards.py`) and `git update-index` in order (`--force-remove` and `--remove` binding the paths after them, `--cacheinfo` and `--index-info` writing an entry over the staged one, `--stdin` every path), each refused where a staged version would be nowhere, as measured on git 2.56.0; two or three trees, `--prefix`, `--index-output`, `-n` and `--again` lose nothing and pass. 5 tests, 4 red on the base; the hooks' own tests 140, all passed.

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
