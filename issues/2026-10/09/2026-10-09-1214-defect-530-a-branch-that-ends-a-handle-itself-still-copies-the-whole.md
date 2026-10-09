---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **530 — a branch that ends a handle itself still copies the whole group array at its first change** | when each `if` branch itself ends a handle, `check` reads 1.49, 3.73 and 10.73 billion instructions at 500, 1,000 and 2,000 after defect 511's repair: a path's first change unshares the flat held array and the `at` map, a copy of every local's group (lane b16-compiler; its generator `.claude/worktrees/scratch-b15/compiler/511/gen2.py`, ignored by git) | `selfhost/check/flow.hero`, `selfhost/check/flow_log.hero` · defect 511 · **class: improvement**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a cost growing faster than the program.
