---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 43d6e237074867d5809b67457a4e3b03ec485afb
github: none
---

- [ ] **342 — an arm of N `|` patterns costs the square of N in `operator_arm.hero`, though the program is correct** | a `match` arm of 1,000 patterns joined by `|` checks in 0.78 billion instructions retired and one of 4,000 in 9.07 billion, four times the input for 11.6 times the cost (lane b11-parse, 2026-10-05, `/usr/bin/time -l` on this Mac, the lane's report) | `selfhost/parse/operator_arm.hero:70` · defects 266 and 267, the same shape of cost · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defects 266 and 267 (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a correct program whose check costs the square of its size; no message or value is wrong.

    Repaired at `43d6e237`, 2026-10-05 (lane b11-parse), the parser's half, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The checker's half stands: `check/reach.after` asks each literal of every one before it and pushes through a record's fields (16,000 patterns, `sample`: `check/walk.arms` into `reach.after`), a file no lane holds, reported.

    **2026-10-05, its checker half** (lane b11-parse's report): for a correct program the square is mostly in the checker, `check/reach.after` scanning every literal seen before and pushing through a record's fields, profiled at 16,000 patterns. This item's cause, so its row, owed before it closes; given to lane b11-parse with `selfhost/check/reach.hero`.
