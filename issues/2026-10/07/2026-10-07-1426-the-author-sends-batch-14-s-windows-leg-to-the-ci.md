---
kind: decision
area: none
milestone: none
filed: 2026-10-07
commit: self
github: none
---

# The author sends batch 14's Windows leg to the CI

2026-10-07, between 14:22 and 14:26 by the clocks read before the question and after the answer, given in the question widget on the coordinator's question *the last Windows leg of batch 14: on the box before the push (the rule), or straight to the CI after the push as for batch 13?*, recommended *the box, before the push*, answered **CI after the push**.

**What it settles, for batch 14's close**: the box's leg on the round's closing commit is not run; the CI's Windows x86-64 job after the push is that leg, as for batch 13 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`). Linux arm64 still runs in Docker before the push. A C-boundary defect of the batch closes once the CI's run on the pushed commit is green on Windows; the other defects at the batch's gate. A lane's single hand-written case on the box, for a defect that exists only there, is not the leg and stays where its lane ran it.

**What it does not settle**: the rule for the batches after 14. `.claude/rules/platforms.md` keeps the box before the push unless the author says this is standing; two batches in a row is a pattern to ask about, not a rule to assume.
