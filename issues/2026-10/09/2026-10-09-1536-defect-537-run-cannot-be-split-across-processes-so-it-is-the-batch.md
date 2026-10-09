---
kind: defect
area: harness
milestone: none
filed: 2026-10-09
commit: none
github: none
---

- [ ] **537 — `run` cannot be split across processes, so it is the batch gate's critical path** | the trial gate of 2026-10-09 read `run` alone at 19:40 of gate B's 19:56, the other 28 suites done in 11:42 at four at a time; split by the coordinator into one harness process per case (422), gate B read 18:39 at seven at a time and a load of 10 on 8 cores, the four seconds each process costs eating the gain, and a one-case run reading its skipped case as a red (the skip ratio); a selector the harness reads itself, a shard *i of n* of a suite's cases, would split it with no process per case | `tests/harness/main.hero` (the third word), `tests/harness/cases.hero` (`matching`), `tests/harness/suite_run.hero` · the optimistic chain, 2026-10-09 · **class: improvement**

    **Origin:** filed by the coordinator at 15:36 on 2026-10-09 under the optimistic chain the author asked for that day (`.claude/rules/verification.md` § The optimistic chain), from batch 16's closing gate (its logs in `.claude/worktrees/scratch-b15/gate16/`, ignored by git) and the record of batches 8 to 15 read that day.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a gate's duration, nobody's correctness.
