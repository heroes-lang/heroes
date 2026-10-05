---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 7419b76fe2817b751d271487b47b18b1e1df3cf2
github: none
---

- [x] **267 — a clean record of N fields costs the square of N, its fields pushed through a growing array** | 2,000 to 8,000 fields: 13.5 times, 30.2 billion instructions at 8,000 (batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/recovery/r178/clean_fields_*`), defect 146's pattern | `selfhost/parse/member_lines.hero:82` (`fields.push`) · **class: adjacent**

    **Origin:** batch 8's recovery lane, instructions retired, 2026-10-03, its report's *Found beside*.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): the lane's reading: a correct program's cost, the pattern a closed defect already named.

    Repaired at `7b63179e` (the parser's half: `member_lines`' pushes, a `.must()` inside the push and two places no push grows in place) and `7419b76f` (the resolver's half: `resolve/state.keyed_add`, a map of maps whose read shared each module's set), 2026-10-04 (lane b11-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    **2026-10-05, the coordinator**: the resolver's half, Repaired at `7419b76f`, is the commit that completes the item, so the card names it; the parser's half at `7b63179e` stands above.

## The repair

Repaired at `7b63179e`, the parser's half, and `7419b76f`, the resolver's. A record's fields were pushed through a growing array, so a clean record of N fields cost the square of N: the parser's members are now matched out before the push and grown in a local, and the resolver's `keyed_add`, a map of maps whose read shared each module's set, uses `name_rows.NameSets`, defect 111's shape. Measured: `check` on a clean record of 8,000 fields from 30.24 to 1.68 billion instructions, the compiler checking itself from 81.44 to 81.21 billion. Its case is `fixedbugs-267-*`, with its `.applied`.

**Closed 2026-10-05** with batch 11 (lanes b11-misc, b11-windows and b11-parse, merged into one round tree made from the trunk at `f5194276`, the trunk merged again at `1bbf2dd7`), its closing gate run on the round's head: the seed regenerated at `97ff7ce8`, 35,479,762 bytes, SHA-256 beginning `f6c1d06c25596494`, its fixpoint by `cmp`, and the compiler's own tests 1,233, all passed; the merge at `1bbf2dd7` moved no line of `selfhost/`, `runtime/` or the seed; then the net's own tests 280, all passed, and the full net, 27 suites and `cache`, 5,365 passed and 0 failed. The census, the trunk's compiler at `f5194276` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,033, 13 moved, the batch's own seven cases and six older goldens its repairs move, each as its repair says; `build --emit-c` over the 623 holding an `extern`, 3 exits moved (the compiler's own source, which the trunk's runtime cannot build, and defect 337's two cases), 2 files of C moved, and 16 of messages, 15 by defect 327's one warning a build and the build cache's key alone, one by defect 311's words. Panel 187's R2, its instrument rebuilt (defect 358), batch 10's compiler, whose source differs from the trunk's at `f5194276` in comments alone, against the round's over one frozen plan: 13,594 single mutants, no class, flag or message count moved in either arm; 16,041 pairs, no second's told status moved. The site's build: 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.70 s against the trunk's 5.68 s, the means of three, and `build --emit-c` 69.33, 69.30 and 69.11 s against the trunk's 70.03 and 70.11 s warm, every run's `real` within 2% of `user` plus `sys`, the C the two emit byte-identical.
