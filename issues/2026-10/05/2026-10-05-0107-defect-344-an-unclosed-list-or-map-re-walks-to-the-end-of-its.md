---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: b93bf7f8212cafd9d80c892ac3a2d10c47139aeb
github: none
---

- [x] **344 — an unclosed list or map re-walks to the end of its declaration for each opener, so N unclosed lines cost the square of N** | `list_line.ended_at_a_binding` and `unclosed.never_closed` walk to the declaration's end once per `[` or `{`: unclosed `[` lines took 46.8 billion instructions at 2,000 and 738.7 billion at 8,000, unclosed `{` lines 24.0 and 375.0 billion (lane b11-parse, 2026-10-05, the lane's report) | `selfhost/parse/list_line.hero` and `selfhost/parse/unclosed.hero`, batch 12's files · defects 266 and 342 · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defect 266's repair (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a broken program whose report costs the square of its openers; the messages are right.

    Repaired at `b93bf7f8`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `b93bf7f8`. The parser asked for an opener's closer by walking from it to the closer, or to the declaration's end for one never closed, once per opener and once per element of a list, and the lexer grew its stack of open brackets through a field place, so N unclosed lines cost the square of N. Every opener of the parse's stream is paired now in one walk keeping the lexer's stack (`pairing.pair_all`), held by a compiler test to the walk from each opener, and `state.push_bracket` grows the stack in place; before the change a compiler comparing the two walks at every lookup checked 2,225 `.hero` files with no mismatch. Measured, instructions retired at 2,000 lines: `x = [1, 2` 47.6 to 0.76 billion, `x = {1: 2` 24.3 to 0.74, `x = f([1` 58.1 to 1.10, a clean list of 2,000 elements 7.3 to 0.39; 8,000 lines 3.76 to 3.84 times 2,000. Its case is `check/fixedbugs-344-every-opener-is-paired-in-one-walk`, the same on the base.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
