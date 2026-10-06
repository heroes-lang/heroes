---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 8c723886bdd7f895bb398dcdf832fe0160b1efda
github: none
---

- [x] **302 — a C loop header whose `(` is never closed takes the body into the bracket, and `missing_body` is told at the end of the block** | `for (i = 0; i < 3; i++` with its `)` left out, a body below: the bracket takes the body, and `missing_body` is told at the block's end rather than at the header (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/loop_habit.hero` · `selfhost/parse/unclosed.hero` · defect 194 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 194.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a mistake told far from where it stands, and a second message for it.

    Repaired at `8c723886`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `8c723886`. `for (i = 0; i < 3; i++` over a body, its `)` left out, let the bracket take the body: the `(` was named still open lines below, the header was told, and `missing_body` was told where the bracket's reach ended or at the block's end. Inside the `(` of C's header holding a `;`, a line that holds no `;` and closes nothing it did not open now ends the bracket's reach (`bracket_reach.below_a_header`), as does a `{` that ends the header's line, so the body is laid out as one; the parser, which tells the header at that `(`, withdraws the lexer's report of it (`parse/unclosed.withdrawn`), the header rewritten whole with its `)`. A header split over lines, each line holding a `;` or its `)`, reads as before. Its case is `check/fixedbugs-302-a-c-header-left-open-keeps-its-body`, red on the base: 24 messages where it says 12.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
