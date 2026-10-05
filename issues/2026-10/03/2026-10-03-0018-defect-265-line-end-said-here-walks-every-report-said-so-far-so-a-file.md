---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 1303e5296c947dc104923c7b2819353f8c501397
github: none
---

- [ ] **265 — `line_end.said_here` walks every report said so far, so a file of refused `while` and `if` heads costs the square of its heads** | failed `while` and `if` heads grow 5.47 times for 4 times the heads (batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/recovery/r184s/while_failed_*` and `if_failed_*`), the shape beside defect 184's `for` heads | `selfhost/parse/line_end.hero:166` (`said_here`) · **class: adjacent**

    **Origin:** batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the lane's reading, a cost beside 184's, whose cause is another function.

    **Cause found 2026-10-04, lane b11-parse**: at 16,000 heads every sample of the square is `line_end.said_here`'s walk over every report said, asked at each `expected_expression` by `grammar_expr.primary` (`selfhost/grammar_expr.hero`). `said_here` has only the cursor's reports to ask, and they are said out of the text's order (an opener's `unclosed_bracket` after the reports inside it), so no early stop rests on the value: a sublinear answer needs the cursor to keep where its reports stand as each is appended (`selfhost/cursor.hero`, and the parse modules that append to `c.diagnostics` directly), not this lane's files. Not repaired; the three walks that asked the question are one since `7b63179e` (`line_end.said_at`), where such an index would answer all three.

    Repaired at `1e8b2315`, 2026-10-05 (lane b11-parse, its files widened to `cursor.hero` that day), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The three appends in batch 12's files (`loop_habit`, `list_line`) stay plain and are walked as the marks' tail.

    **2026-10-05, the sites the marks do not reach** (lane b11-parse's report): three plain appends in batch 12's files, one in `parse/loop_habit.hero` and two in `parse/list_line.hero`, are walked as a tail rather than marked, so their reports keep the walk's cost. This item's cause, so its rows, owed before it closes, by the lane that holds those files in batch 12.

    Repaired at `1303e529`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
