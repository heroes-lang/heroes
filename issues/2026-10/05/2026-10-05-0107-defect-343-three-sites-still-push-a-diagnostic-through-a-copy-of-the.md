---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: e09550e75fe22af3f2f79190a3b1dc17a29f3943
github: none
---

- [x] **343 — three sites still push a diagnostic through a copy of the whole list, so N line ends or N bare function types cost the square of N** | `c.diagnostics @ c.diagnostics.push(d)` in `line_end.refused`, `type.bare_function` and `type.named_parameter`: N line ends before an operator went 9.0 times the instructions and N bare function types 5.5 times for four times the input (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/parse/line_end.hero` and `selfhost/parse/type.hero` (the three pushes) · `cursor.push_diagnostic`, which grows in place · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 267's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a broken program whose report costs the square of its mistakes; the messages are right.

    Repaired at `e09550e7`, 2026-10-05 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The line ends' rest was `said_here`'s walk, repaired with defect 265 at `1e8b2315`.

## The repair

Repaired at `e09550e7`. `line_end.refused`, `type.bare_function` and `type.named_parameter` pushed a diagnostic through a copy of the whole list; they append in place. Measured: 4,000 bare function types from 8.29 to 5.02 billion instructions, 4,000 labels that are not names from 9.62 to 6.35. Its case is `fixedbugs-343-*`, with its `.applied`. Five such sites outside the parser are defect 350.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
