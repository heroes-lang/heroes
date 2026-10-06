---
kind: decision
area: none
milestone: none
filed: 2026-10-06
commit: self
github: none
---

# The author's goal: every defect but the improvements closed within a day, and the roadmap advanced

2026-10-06, given in conversation at 11:44 by the clock read then, meant as: *I would like to give myself this goal: within 24 hours, finish and close every defect that is not an improvement, and advance the roadmap, doing the buildable structs and the issue files too.* And minutes later, meant as: *you must learn to parallelise a lot, above all when you have things running on Windows, on those slow machines.*

What it reads as, measured at 11:43 from the cards: 52 open defects are not improvements (10 `blocking`, 2 `systemic`, 40 `adjacent`), and every one of them is in batch 12: 47 carry a repair line and await the batch's gate, 305 closes as not reproduced, 381 is repaired in lane ir12, 387 and 385 are in their lanes, and 382 keeps one route that goes to the panel's soundness lane. Defects 091 to 094, panel 178's, join when they are re-run. **M-issue-files** holds five open tasks, four of them acts on GitHub, which are outward-facing and asked for each time (CLAUDE.md § Hard stops); **M-buildable-structs** opens after it, since `site/src/lib/chain.ts` admits one open row, on panel 194, which sits panel 178 again on the code of this day.

The parallel instruction lands as a line of `.claude/rules/platforms.md` at batch 12's close, with that batch's two other rule edits: a leg on a slow machine starts as soon as its input exists, beside everything else, and this Mac never waits idle on it.
