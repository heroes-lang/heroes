---
kind: defect
area: parse
milestone: none
filed: 2026-10-06
commit: 2cd107a7499f59866e9ea956b219960fdeda20eb
github: none
---

- [x] **375 — an expression broken around an operator alone on its line costs two messages** | `k = n` over a deeper `-` over a deeper `1`: `continuation_outside_brackets` and `unexpected_block`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator at 02:46 on 2026-10-06, `<scratchpad>/batch12/filing/` `n4.hero`); defect 370 leaves the operand-below shape alone on purpose | `selfhost/open_line.hero`, `selfhost/sign_above.hero` · defect 370's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-06, found beside defects 363 and 370 (its report's *found beside*); reproduced by the coordinator at 02:46 and filed into batch 12 under the author's instruction of 2026-10-05, any defect found that is not an improvement goes into the batch.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    Repaired at `2cd107a7`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `2cd107a7`. `k = n` over a deeper `-` over a deeper `1`, an expression broken around its operator alone on its line, cost `continuation_outside_brackets` at the operator's line end and `unexpected_block` at its margin. Operators alone on a line laid out deeper than a statement that opens no block, whose next line is their operand, now break one expression in two places, told once at the operators with the join of the three lines offered, `k = n - 1`, certain where the operand cannot stand alone (`selfhost/alone_lines.hero`); at the statement's own margin defect 129's reading stands. Its case is `check/fixedbugs-375-an-expression-broken-around-its-operator-is-told-once`, red on the base: 23 messages; 13 now, one per operator line and the shapes as before; the join itself is held by `joined_lines.hero`'s test.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
