---
kind: defect
area: harness
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **536 — a suite's floor turns gate B red whenever the tree outgrows it by a quarter** | `tests/harness/floors.hero` checks a floor from both sides: a count below it is red, and so is a count past it by more than `SLACK` (25%), whose repair is to raise the floor in the same commit; of gate B's about 26 reds over batches 8 to 15, about 24 were a floor or an expectation raised to the merged tree's count and none was a defect of the compiler. The optimistic chain (item 6) asks that the floors stop turning a batch red while the decay side, the half that caught a walk missing cases on 2026-09-12, stays: a floor raised by the batch's closing itself, a wider slack, or the high side told and not red, is this item's question | the floors of `tests/harness/suite_*.hero` (`LEAST`, the `floor` constants of `suite_golden.hero`) · the optimistic chain, 2026-10-09 · **class: improvement**

    **Origin:** filed by the coordinator at 15:36 on 2026-10-09 under the optimistic chain the author asked for that day (`.claude/rules/verification.md` § The optimistic chain), from batch 16's closing gate (its logs in `.claude/worktrees/scratch-b15/gate16/`, ignored by git) and the record of batches 8 to 15 read that day.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): an instrument that reds on growth, the gate's most frequent red.
