---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: ce781e5452e3bccb9aefec21770627d993d413a8
github: none
---

- [x] **303 — a `}` followed by `while (c)` on the next line, C's do-while split across lines, costs two messages** | a `}` closing a block, then `while (c)` on the line below: two messages, where defect 201's repair tells `} while (c)` on one line as one habit (lane b9-recovery's compiler, 2026-10-04) | `selfhost/parse/braced_lines.hero` · defect 201 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 201.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake.

    **Cause found 2026-10-04, lane b11-parse**: a `}` alone on its line is no token, the lexer having laid out the braces of a `{` that ends its line (`brace_layout.hero`, ruling 1; `heroes lex --dump-tokens`: `dedent` then `while`), so `loop_habit.past_a_statement`, which tells the habit where `c.tokens[close + 1]` is the `while`, and `loop_habit.do_tail`, which reads the tail only with its `}` on the `while`'s line, see neither. Both are in `selfhost/parse/loop_habit.hero`, batch 12's file. Not repaired.

    Repaired at `ce781e54`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `ce781e54`. C's do-while with its `}` alone on a line and `while (c)` below cost two messages, the `do {`'s `{` as a brace past a name and the `while`'s missing body: a `}` alone on its line is no token, the lexer having laid out the braces, so `loop_habit.past_a_statement` and `do_tail` saw neither. Past the block the lexer laid out below the `{` stands the habit's tail now, its `{` found by the block's depth (`loop_habit.laid_opener`), empty braces included; and `do_tail` looks past the line's end for a body, so `} while (c)` over a body, which cost the habit and `unexpected_block` on the trunk too, reads as the loop with its body. Its case is `check/fixedbugs-303-a-do-while-split-over-lines-is-one-habit`, red on the base: 18 messages where it says 10.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
