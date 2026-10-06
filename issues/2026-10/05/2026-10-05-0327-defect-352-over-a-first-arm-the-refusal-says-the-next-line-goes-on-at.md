---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 7c6f214498449954c2aa328dd5f476ad9ead6a1a
github: none
---

- [x] **352 — over a first arm, the refusal says the next line goes on *at the same margin*, naming the level after the jump cap rather than the margin as written** | a continuation refused over a `match`'s first arm: the message's *at the same margin* names the level the parser capped the jump at, not the margin the line was written at; the base says the same (lane b11-parse, 2026-10-05, the lane's report) | the parser's continuation message · defect 268 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 268's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `7c6f2144`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `7c6f2144`. A line refused for an end it cannot end with, written deeper than the layout lays it out, said the next line goes on with it *at the same margin, where a new statement begins*, where the author wrote that line shallower, as over a `match`'s first arm: the message compared the next line with the level the layout capped the jump at, or read five spaces as one level. It compares now with the margin the statement's first line was written at, where that margin is spaces alone and no braces or indentation habit keep their own count (`open_line.written_margin`). Defect 182's `before_the_first_arm`, 268's `over_the_first_arm` and 351's `the_second_shallower` move with it, each told *at a shallower margin* under a dated note. Its case is `check/fixedbugs-352-a-refused-line-end-names-the-margin-as-written`, with its `.applied`, red on the base: five *same margin* where the margins as written differ.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
