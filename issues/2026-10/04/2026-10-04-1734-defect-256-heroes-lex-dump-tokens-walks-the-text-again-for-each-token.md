---
kind: defect
area: cli
milestone: none
filed: 2026-10-03
commit: 51406d017c0faa947aa3a0e1b600d36b17edc5a3
github: none
---

- [x] **256 — `heroes lex --dump-tokens` walks the text again for each token it prints** | 24,052,192 characters walked on `concat-chain-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/cli/lex.hero:50` and `:68` · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the file's square on one verb; no program refused or wrong.

    Repaired at `51406d01`, 2026-10-04 (lane b10-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `51406d01`. `lex --dump-tokens` asked `source.locate` for each token, which counts a column's characters from the start of the line, so every line was walked once per token on it. One forward walk now places each token. Instructions retired on this Mac, a count and no duration: one line of 2,000 joined strings, 30.38 billion before and 0.186 billion after; 4,000 lines of one statement, 1.025 billion and 0.783 billion. The dumps are the same bytes, 1,436 of them compared by `cmp`. Its case is a compiler test placing every token of a text with characters of one to four bytes and a tab.

**Closed 2026-10-04** with batch 10 (lanes b10-ir, b10-cli and b10-harness, merged into one round tree with the trunk at `761525bb`), its closing gate run on the round's head: the seed regenerated at `a134aa74`, 35,206,983 bytes, SHA-256 beginning `c79ffd5ad005c301`, its fixpoint by `cmp`, and the compiler's own tests 1,213, all passed; panel 191's sitting merged at `1dad1ac9`, no line of `selfhost/`, `runtime/`, `tests/` or the seed moving between; then on `1dad1ac9` the net's own tests 260, all passed, and the full net, 27 suites and `cache`, 5,321 passed and 0 failed. The census, the trunk's compiler at `761525bb` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,014, 3 moved, defect 325's own cases; `build --emit-c` over the 621 holding an `extern`, every exit the same, 34 files of C moved by defect 231's one exit and 15 of messages by the build cache's key alone. Panel 187's R2, the trunk's compiler against the round's over one frozen plan: 13,594 single mutants, none with more or fewer messages in either arm, 26,329 readings differing by the mutant's file name alone; 15,842 pairs, no told second hidden. The site's build: 188 pages, 36 claims and 2 verb lists checked. The clock, on a still machine over the trunk's compiler source: `check` 5.64 s against the trunk's 5.63 s, and `build --emit-c` 69.4 and 70.2 s against 75.0 and 75.9 s.
