---
kind: defect
area: harness
milestone: none
filed: 2026-10-09
commit: 2705815ee358bb2d9a1afc878e97e3c5b26eb0ae
github: none
---

- [ ] **537 — `run` cannot be split across processes, so it is the batch gate's critical path** | the trial gate of 2026-10-09 read `run` alone at 19:40 of gate B's 19:56, the other 28 suites done in 11:42 at four at a time; split by the coordinator into one harness process per case (422), gate B read 18:39 at seven at a time and a load of 10 on 8 cores, the four seconds each process costs eating the gain, and a one-case run reading its skipped case as a red (the skip ratio); a selector the harness reads itself, a shard *i of n* of a suite's cases, would split it with no process per case | `tests/harness/main.hero` (the third word), `tests/harness/cases.hero` (`matching`), `tests/harness/suite_run.hero` · the optimistic chain, 2026-10-09 · **class: improvement**

    **Origin:** filed by the coordinator at 15:36 on 2026-10-09 under the optimistic chain the author asked for that day (`.claude/rules/verification.md` § The optimistic chain), from batch 16's closing gate (its logs in `.claude/worktrees/scratch-b15/gate16/`, ignored by git) and the record of batches 8 to 15 read that day.

    **Class: improvement**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a gate's duration, nobody's correctness.

    Repaired at `2705815e`, 2026-10-09 (lane b17-fix, batch 17), and at `c8c7ea9c` a comment `records/citations` read as a citation; gated by its cases and the net's own tests; the net is owed at the batch's close. The harness's third word takes a second form, `<i>/<n>`: shard `i` of `n` of `run` or a golden form, its cases dealt round by their sorted index, each case in exactly one shard; a shard asks neither the floor nor the skip ratio and its line says so, a word in the shape out of range and a shard of `annotations`, `fixes` or `layout` exit 2. Tests: the shards of every `n` to past the cases' count disjoint, their union the suite, none holding two more than another; the net's own tests 326; `ir` whole 29 and its three shards 10, 9 and 9. Measured that evening, `run` alone with the seed's compiler, warm after one six-shard pass (881.8 s cold), each shard its own harness process: whole 1,573.6 s `real` against 357.7 user and 161.9 sys, begun at a load of 89 from other sessions' builds, so discarded as a duration; 2 shards 668.2 s (402.7 of CPU) and 4 shards 546.3 s (401.9), both beside this lane's own builds and gates, so a ceiling rather than a reading; 6 shards 561.6 s (409.4), beside nothing of this lane's at a load of 3 to 4. Every reading has `real` far above its CPU over `n`, so `run` waits rather than computes, and 4 and 6 shards reading alike says a few slow cases bound a shard, an inference. The shards' counts summed to the whole each time: 421 passed and the one case this Mac skips (437's, `sys/prctl.h`), 422 of 422. `scratch-b15/gate-b4.sh` (ignored by git) runs gate B with `run` in 4 shards queued first in gate-b3's pool of 5, and asks the whole suite's questions of the shards' sums.
