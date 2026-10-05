---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 1684bab5c853c64b2c162cc166700cec7c6aa69a
github: none
---

- [x] **268 — a `-` alone on a line deeper than an integer arm is told twice, the second an `unexpected_block` true only if the `-` joins the next line** | `k = match n` over `2 => "two"`, a line `-` one level deeper, then `1 => "one"` and `_ => "many"`: `check` exit 1, `continuation_outside_brackets` at 5:13, then `unexpected_block` at 5:1 (batch 8's round compiler at `1eb854c3`, 2026-10-04, `<scratchpad>/batch8/recovery/shapes/s182_minus_over_int.hero`) | `selfhost/parse/` (the stray operator's margin, beside defect 182's repair) · **class: adjacent**

    **Origin:** batch 8's recovery lane, 2026-10-03 (its report's *Found beside*); reproduced by the coordinator, 2026-10-04.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, the stray `-`.

    **Cause found 2026-10-04, lane b11-parse**: where both readings stand, `sign_above.offer` hands the parser the join with the `-` line's margin, an indent the arm below is joined into, so `grammar_expr.arms_of` names an orphan (`parse/orphans.read`); two levels deeper the lexer's own `indentation_jump` stands too, three messages, and the join fix, from the `-`'s end to the arm's first byte, writes the arm at the `-`'s margin, a block deeper than anything that opens one. Defect 182's move, the margin taken back, made for two readings is the lexer's (`selfhost/sign_above.hero`), not this lane's file; a hold in the parser could not take back the lexer's own margin reports. Not repaired.

    Repaired at `1684bab5`, 2026-10-05 (lane b11-parse, its files widened to `sign_above.hero` that day), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A tab margin under the `-` keeps its reports, as defect 182's does, and two stray `-` lines in a row are a shape beside, not at depth one.

## The repair

Repaired at `1684bab5`. Where a `-` alone on a line deeper than an integer arm had two readings, the lexer's `sign_above.offer` kept the `-` line's margin, so the arm below was read joined into it and told twice; `sign_above.signed_below` now takes back that margin and lays out the arm's own, so the arm is read where it stands, and the join fix writes the sign on the arm's line. Its case is `fixedbugs-268-*`, six shapes at one message each where the base said twelve. Two stray `-` lines in a row are defect 351.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
