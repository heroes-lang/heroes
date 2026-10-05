---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 627c64db4fcb72d848aa2891419b22ca812dd34c
github: none
---

- [x] **311 — a `fn` among a record's fields is told once since defect 200's repair, and the message no longer says to take the record as a parameter** | `fn` written among a record's fields, the record twin of defect 200: two messages before, one since `7831aec7`, and that one has lost *taking the record as a parameter*, the route the old second message gave (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/member_lines.hero` · defect 200's repair, `7831aec7` · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`); the lane's own repair's, reported.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message that lost a route it carried; batch 9's own change, read at its gate.

    **Cause found 2026-10-04, lane b11-parse**: the one message is the lexer's `reserved_word`, whose words `foreign_at.misplaced` writes for a place it reads as a body or another declaration (`selfhost/foreign_at.hero`), and `parse/heads.next_member` stays silent where the lexer spoke (`7831aec7`). One message carrying *taking the record as a parameter* needs the lexer's words to name the route, or the parser's message to replace the lexer's where the two meet (`selfhost/parse.hero`, `parse/unclosed.withdrawn`), none of them this lane's files. Not repaired.

    Repaired at `627c64db`, 2026-10-05 (lane b11-parse, its files widened to `foreign_at.hero` that day): the lexer's one message names the holder and the route; gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `627c64db`. A `fn`, `def` or `func` among a record's fields or a variant's cases was told by the lexer's `reserved_word` alone, naming no holder; `foreign_at.holder_of` now names the holder and the route, *taking the record as a parameter*. Its case is `fixedbugs-311-*`, with its `.applied`; `fixedbugs-130-a-function-among-a-records-fields` and `fixedbugs-200-*` change words only, each dated under its notes.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
