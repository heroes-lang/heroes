---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: none
github: none
---

- [ ] **265 — `line_end.said_here` walks every report said so far, so a file of refused `while` and `if` heads costs the square of its heads** | failed `while` and `if` heads grow 5.47 times for 4 times the heads (batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/recovery/r184s/while_failed_*` and `if_failed_*`), the shape beside defect 184's `for` heads | `selfhost/parse/line_end.hero:166` (`said_here`) · **class: adjacent**

    **Origin:** batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the lane's reading, a cost beside 184's, whose cause is another function.

    **Cause found 2026-10-04, lane b11-parse**: at 16,000 heads every sample of the square is `line_end.said_here`'s walk over every report said, asked at each `expected_expression` by `grammar_expr.primary` (`selfhost/grammar_expr.hero`). `said_here` has only the cursor's reports to ask, and they are said out of the text's order (an opener's `unclosed_bracket` after the reports inside it), so no early stop rests on the value: a sublinear answer needs the cursor to keep where its reports stand as each is appended (`selfhost/cursor.hero`, and the parse modules that append to `c.diagnostics` directly), not this lane's files. Not repaired; the three walks that asked the question are one since `7b63179e` (`line_end.said_at`), where such an index would answer all three.
