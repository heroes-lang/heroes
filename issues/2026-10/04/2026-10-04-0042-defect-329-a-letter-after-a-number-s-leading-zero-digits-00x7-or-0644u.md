---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 3f032bc7b4a35890406a2ac4a657f25a3ccee832
github: none
---

- [x] **329 — a letter after a number's leading-zero digits, `00x7` or `0644u`, is told twice: `leading_zero` and `expected_end_of_line`** | `x = 00x7` and `x = 0644u`: each gets `leading_zero` at the number and `expected_end_of_line` at the letter, two messages for the one mistake; defect 324's goldens (`check/fixedbugs-324-*`) pin both as they stand | `selfhost/number.hero` and `selfhost/leading_zeros.hero` (where the leading zero's reading stops) · defect 324 · **class: adjacent**

    **Origin:** lane b10-cli, 2026-10-04, beside 324, on its compiler at `56cb4513` (its reply after 324).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `3f032bc7`, 2026-10-05 (lane b11-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The shapes beside with this cause were repaired with it: other letters, a character beyond ASCII and a fraction after the digits, and the literal inside a call's or a list's brackets, where the second message was `expected_args_close` or `expected_separator`.

## The repair

Repaired at `3f032bc7`. A letter after a number's leading-zero digits, `00x7` or `0644u`, was told twice, `leading_zero` and `expected_end_of_line`; whatever joins the digits now belongs to the refused literal (`leading_zeros.joined_end`), told once, and so for `0644u8`, `007e3`, `0_7u`, `00b1`, `0644é`, `00x7.5`, `print(0644u)` and `[00x7, 1]`. Its case is a new `check/` case; defect 324's two goldens move, each corrected underneath with its date.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
