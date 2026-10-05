---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: 368263a83e9764df934eb2ad1bfbb4d6e42d61f8
github: none
---

- [x] **339 — `1_u`, `0644_u` and `00_x7` are told twice, `misplaced_separator` and then `expected_end_of_line`** | `x = 1_u`, `x = 0644_u`, `x = 00_x7`: each `check`s to two messages for the one mistake, the separator's and then the line's end at the letter (lane b11-misc's compiler, 2026-10-05, the lane's report) | `selfhost/number.hero` (the separator scan) · defect 329, which made a leading zero's joined letters one message · **class: adjacent**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 329's repair (its final report, *Found beside*): the cause is in the separator scan, not in 329's.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, not one of `blocking`'s list.

    Repaired at `368263a8`, 2026-10-05 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `368263a8`. A decimal number whose `_` is misplaced was told `misplaced_separator` and then `expected_end_of_line` at the letter after it; the letters now belong to the same refused literal, so `1_u`, `0644_u`, `00_x7` and `print(1_u8)` each give one message where the base gave two. Its case is `check/fixedbugs-339-*`, four lines marked, and a compiler test.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
