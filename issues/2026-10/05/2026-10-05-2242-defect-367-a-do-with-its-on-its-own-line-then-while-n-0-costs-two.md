---
kind: defect
area: parse
milestone: none
filed: 2026-10-05
commit: f1344c53563cd5666d35bb6d02ebf7dbba46ca99
github: none
---

- [x] **367 — a `do` with its `{` on its own line, then `while (n > 0)`, costs two messages that name neither** | `do` over `{`, `print(n)`, `}`, `while (n > 0)`: `expected_expression` and `missing_body`, on the base and on lane b12-parse12's compiler alike (measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `303-h-allman.hero`) | `selfhost/parse/braced_lines.hero`, `parse/loop_habit.hero` · defect 303's sibling · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, neither naming the C habit.

    Repaired at `f1344c53`, 2026-10-06 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `f1344c53`. C's do-while with its `{` on a line of its own, `do` over `{`, its body, `}` and `while (n > 0)`, cost `expected_expression` at the block, read as a map's entry, and `missing_body` for the `while`, naming neither; with no braces at all, `unexpected_block` and the same `missing_body`. The name `do` alone on its line over a block, braces the lexer laid out or an indented block, is C's do-while now, told once at the block (`parse/loop_habit.under_a_do`); the block is read as a body and the `while` below it as its tail, whose missing body is asked for no more. A name `do` over no block is a name, as before. Its case is `check/fixedbugs-367-a-do-over-a-block-below-it-is-told-once`, with its `.applied`, red on the base: 15 messages; 8 now, one per `do` and the tail's own `;`.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
