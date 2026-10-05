---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: e703a84308526fadf15b7ef92b1a5c866705a549
github: none
---

- [x] **338 — for `00x` and `00xg` the `leading_zero` advice writes `0x` and `0xg`, two literals the compiler refuses** | `x = 00x` and `x = 00xg`: `leading_zero`'s guess fix `0` applied writes `0x` and `0xg`, each refused at `check` (lane b11-misc's compiler, after `3f032bc7`, 2026-10-05, the lane's report) | `selfhost/leading_zeros.hero` (the advice) · defect 324, whose repair made every piece of the advice a literal the compiler accepts · defect 329 · **class: blocking**

    **Origin:** lane b11-misc, 2026-10-05, beside defect 329's repair (its final report, *Found beside*); 324's cause, the advice writing a program the compiler refuses, at a shape 324's cases do not hold.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a `guess` fix that writes a program the compiler refuses, defect 324's own class reading (`.claude/rules/verification.md` § Bounded discovery), so never deferred: repaired in this batch.

    Repaired at `e703a843`, 2026-10-05 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside with this cause were repaired with it: a digit no number of the marker's base has (`00b2`, `00o9`), a fraction, and a separator that leads or trails after the marker.

## The repair

Repaired at `e703a843`. For `00x` and `00xg` the `leading_zero` advice wrote `0x` and `0xg`, literals the compiler refuses; one zero before a base's marker is now offered only where `0`, the marker and what follows are a literal the compiler accepts, and otherwise a note says why no fix is offered and how that base's literal is written. A compiler test holds the check (`leading_zeros.based_literal`) to the scanner's own answer over 13 shapes; its case is `full/fixedbugs-338-*`, three lines marked.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
