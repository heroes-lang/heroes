---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: e08488a52a04851bd433960ac028973cc376525b
github: none
---

- [x] **301 — a bare C loop header, `for i = 0; i < 3; i++`, costs four messages** | `for i = 0; i < 3; i++` with no parentheses: four messages, where defect 194's repair tells the parenthesised header once (lane b9-recovery's compiler, 2026-10-04) | `selfhost/scan.hero` (the lexer's `;`), `selfhost/parse/loop_habit.hero` · defect 194 · **class: adjacent**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), beside 194; by the lane's reading another cause, the bare header never reaching the loop habit's parenthesised reading.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): several messages for one mistake; if its cause is 194's own, 194 reopens.

    Repaired at `e08488a5`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `e08488a5`. `for i = 0; i < 3; i++` with no parentheses cost four messages: the lexer's `unexpected_character` at each `;`, its `continuation_outside_brackets` at the `++` that ends the line, and the parser's `for_missing_in`. A `;` on a line that begins with `for` and holds no `in` before it is the header's now, taken with the rest of its line (`closers.header_semicolon`), and the loop's report reads it (`parse/loop_habit.c_header`) and tells it once, *C's three-clause header is no loop here*, with no fix, at the loop variable, or at the `;` where none stands before it. Its case is `check/fixedbugs-301-a-bare-c-for-header-is-told-once`, with its `.applied`, red on the base: 29 messages where it says 16, one header each and beside it only a mistake of its own.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
