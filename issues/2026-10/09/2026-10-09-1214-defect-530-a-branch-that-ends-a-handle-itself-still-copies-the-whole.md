---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 3248ea843ab5039ba2eb1d06d4db7c28cde9a922
github: none
---

- [ ] **530 — a branch that ends a handle itself still copies the whole group array at its first change** | when each `if` branch itself ends a handle, `check` reads 1.49, 3.73 and 10.73 billion instructions at 500, 1,000 and 2,000 after defect 511's repair: a path's first change unshares the flat held array and the `at` map, a copy of every local's group (lane b16-compiler; its generator `.claude/worktrees/scratch-b15/compiler/511/gen2.py`, ignored by git) | `selfhost/check/flow.hero`, `selfhost/check/flow_log.hero` · defect 511 · **class: improvement**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing faster than the program.

    Repaired at `3248ea84`, 2026-10-09 (lane b16-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The groups are a tree of 8 a node indexed by the local's number (`check/flow_groups.hero`), so a path's first change copies one node a level, and what changed since a split is each group's stamp of its path's clock, a node keeping the latest below it (`check/flow_log.hero`), where the record had been a list and a map that a path's first change copied too. Measured beside the card on four more shapes, `check` instructions at 500, 1,000, 2,000 and 4,000 before and after: the card's 1.49, 3.74, 10.73 and 34.63 billion to 1.10, 2.14, 4.21 and 8.36; inside a loop 2.68, 8.36, 29.05 and 107.28 to 1.21, 2.36, 4.66 and 9.27; `match` arms and `&&` alike; an end on one branch and a handle opened in each branch, linear before, 1 and 2.8 per cent dearer at 4,000; the compiler checking itself within 0.1 per cent. Verdicts byte-identical over the 224 tracked handle programs and the 27 generated ones; one new `check` case pins them.
