---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: 2078ceb18ec03e572120266a53e153046cc26b95
github: none
---

- [x] **351 — two stray `-` lines in a row under an integer arm are still told three messages** | a `match` whose integer arm is followed by two lines holding a `-` alone, each deeper: `check` tells three messages for them where defect 268's repair tells one for a single such line (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/sign_above.hero` · defect 268 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 268's repair (its final report, *Found beside*), a shape beside the item's own.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second and third message for what may be one mistake; the lane reads it apart from 268's cause.

    Repaired at `2078ceb1`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `2078ceb1`. Two lines in a row, each holding a `-` alone, under an integer arm and deeper, were told each line's `continuation_outside_brackets` and `unexpected_block` at the first line's margin, where defect 268 tells one such line once: its refusal held only where the next line with words held the arm's `=>`, so the first of two was joined to the second and its margin read as a block. Past lines that each hold an operator alone, the arm below is now the arm they all stand above (`next_line.arrow_below`): each line is refused once, the first ones' deletion certain, the last's the two readings of 268, every margin taken back. Its case is `check/fixedbugs-351-stray-minus-lines-over-an-arm-are-told-one-each`, with its `.applied`, red on the base: 21 messages, six `unexpected_block` among them; 14 now, one per stray line.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
