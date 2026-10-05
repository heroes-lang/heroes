---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f0f8c6252937693e9ef17be94d3d48418422ecae
github: none
---

- [x] **342 — an arm of N `|` patterns costs the square of N in `operator_arm.hero`, though the program is correct** | a `match` arm of 1,000 patterns joined by `|` checks in 0.78 billion instructions retired and one of 4,000 in 9.07 billion, four times the input for 11.6 times the cost (lane b11-parse, 2026-10-05, `/usr/bin/time -l` on this Mac, the lane's report) | `selfhost/parse/operator_arm.hero:70` · defects 266 and 267, the same shape of cost · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defects 266 and 267 (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a correct program whose check costs the square of its size; no message or value is wrong.

    Repaired at `43d6e237`, 2026-10-05 (lane b11-parse), the parser's half, gated by its cases and the compiler's own tests; the net is owed at the batch's close. The checker's half stands: `check/reach.after` asks each literal of every one before it and pushes through a record's fields (16,000 patterns, `sample`: `check/walk.arms` into `reach.after`), a file no lane holds, reported.

    **2026-10-05, its checker half** (lane b11-parse's report): for a correct program the square is mostly in the checker, `check/reach.after` scanning every literal seen before and pushing through a record's fields, profiled at 16,000 patterns. This item's cause, so its row, owed before it closes; given to lane b11-parse with `selfhost/check/reach.hero`.

    Repaired at `f0f8c625`, 2026-10-05 (lane b11-parse, its files widened to `check/reach.hero` that day), the checker's half: the values taken looked up in a map, 16,000 patterns 118.61 to 2.24 billion instructions retired; gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `43d6e237`, the parser's half, and `f0f8c625`, the checker's. An arm of N `|` patterns cost the square of N: `operator_arm.lead` now matches each pattern out before pushing it, and `check/reach.after`, which scanned every literal value taken before each pattern and grew two arrays through a record's fields, holds a map from each value to its first spelling. Measured: `check --brief` over 16,000 patterns from 118.61 to 2.24 billion instructions, 4,000 to 16,000 now 3.54 times for four times the input; the reports byte-identical. Its cases are `fixedbugs-342-*` and `check/fixedbugs-342-the-values-an-arms-patterns-take-are-looked-up`.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
