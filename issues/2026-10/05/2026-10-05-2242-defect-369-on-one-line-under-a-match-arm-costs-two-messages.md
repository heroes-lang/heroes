---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f527a1efd4b011636a918f974bdd61669d178c6c
github: none
---

- [x] **369 — `- -` on one line under a match arm costs two messages** | `- -` on its own line under an arm of a `match` over an integer: `continuation_outside_brackets` and `unexpected_block`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `351-g-one-line.hero`) | `selfhost/sign_above.hero` · defect 351's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `f527a1ef`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `f527a1ef`. `- -` on one line under an arm of a `match` over an integer was told `continuation_outside_brackets` at its end and `unexpected_block` at its margin, where defect 351 tells a line holding one operator once: a line was asked whether it held an operator alone above an arm by its last token alone. Operators alone on a line are a run now, walked back from the line's end (`sign_above.stray`, `alone_from`) and skipped on the lines between as one (`next_line.past_operators`): above an arm the line is refused once, its whole line deleted, certain, since no pattern begins with two operators. One operator alone keeps defects 159's, 165's, 166's, 182's, 268's and 351's readings. Its case is `check/fixedbugs-369-operators-alone-over-an-arm-are-one-stray-line`, with its `.applied`, red on the base: 18 messages; 9 now, one per stray line.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
