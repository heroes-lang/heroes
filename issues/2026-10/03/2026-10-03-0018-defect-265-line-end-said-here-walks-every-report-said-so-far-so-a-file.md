---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 1303e5296c947dc104923c7b2819353f8c501397
github: none
---

- [x] **265 — `line_end.said_here` walks every report said so far, so a file of refused `while` and `if` heads costs the square of its heads** | failed `while` and `if` heads grow 5.47 times for 4 times the heads (batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/recovery/r184s/while_failed_*` and `if_failed_*`), the shape beside defect 184's `for` heads | `selfhost/parse/line_end.hero:166` (`said_here`) · **class: adjacent**

    **Origin:** batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the lane's reading, a cost beside 184's, whose cause is another function.

    **Cause found 2026-10-04, lane b11-parse**: at 16,000 heads every sample of the square is `line_end.said_here`'s walk over every report said, asked at each `expected_expression` by `grammar_expr.primary` (`selfhost/grammar_expr.hero`). `said_here` has only the cursor's reports to ask, and they are said out of the text's order (an opener's `unclosed_bracket` after the reports inside it), so no early stop rests on the value: a sublinear answer needs the cursor to keep where its reports stand as each is appended (`selfhost/cursor.hero`, and the parse modules that append to `c.diagnostics` directly), not this lane's files. Not repaired; the three walks that asked the question are one since `7b63179e` (`line_end.said_at`), where such an index would answer all three.

    Repaired at `1e8b2315`, 2026-10-05 (lane b11-parse, its files widened to `cursor.hero` that day), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The three appends in batch 12's files (`loop_habit`, `list_line`) stay plain and are walked as the marks' tail.

    **2026-10-05, the sites the marks do not reach** (lane b11-parse's report): three plain appends in batch 12's files, one in `parse/loop_habit.hero` and two in `parse/list_line.hero`, are walked as a tail rather than marked, so their reports keep the walk's cost. This item's cause, so its rows, owed before it closes, by the lane that holds those files in batch 12.

    Repaired at `1303e529`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `1e8b2315`, the cursor's marks, and `1303e529`, the three appends left in batch 12's files. `line_end.said_here`, asked at each missing expression and closer, walked every report said so far, so a file of refused `while` and `if` heads cost the square of its heads. The cursor now marks where each report stands as it is said (`report_marks`, through `cursor.say`), and `loop_habit.refuse`, `list_line.set_apart` and `list_line.trailing_comma` say through it too. Measured, instructions retired by `check --brief`: 8,000 `while` heads 25.46 to 6.81 billion, `if` heads the same, 4,000 line ends 10.52 to 2.49, 8,000 lists of `[1` over `- 2]` 9.53 to 4.77; `check selfhost/main.hero` 81.50 to 81.43 billion; outputs byte-identical over eight shapes. Its cases are `check/fixedbugs-265-a-report-on-a-token-is-found-by-its-mark` and `check/fixedbugs-265-the-appends-left-in-the-loop-and-list-readers-are-marked`, each with its `.applied`.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
