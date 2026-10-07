---
kind: decision
area: none
milestone: none
filed: 2026-10-07
commit: self
github: none
---

# The author sends batch 13's last Windows leg to the CI

2026-10-07 at 10:19 by the clock read then, given in conversation after the coordinator's summary that the final Windows leg on the box takes about an hour and a half and would close the C-boundary defects after the goal's 11:44, meant as: *you know what, the very last Windows round, we go optimistically straight to the CI*.

**What it settles, for batch 13's close**: the box's final leg on the closing commit is not run; the CI's Windows x86-64 job after the push is that leg. Linux arm64 still runs in Docker before the push (`.claude/rules/platforms.md`). A C-boundary defect of the batch closes once the CI's run on the pushed commit is green on Windows, the other defects at the batch's gate as always. The box's pre-leg on the round at `0dd1cfd8`, already running at 10:19, is left to finish and read as evidence, not as the leg.

**What it does not settle**: the rule for the next batch. `.claude/rules/platforms.md` keeps the box before the push unless the author says this is standing.
