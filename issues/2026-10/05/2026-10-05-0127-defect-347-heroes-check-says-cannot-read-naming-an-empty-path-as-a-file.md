---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: 35ff902445ea96a7b9567267a14d8b8e5efde753
github: none
---

- [x] **347 — `heroes check ''` says *cannot read ``*, naming an empty path as a file** | `heroes check ''`: *cannot read ``*, where an empty argument names no file at all (lane b11-windows, this Mac, 2026-10-05, the lane's report) | `selfhost/cli/input.hero` (the file read) · defect 295, which told a missing file from an unreadable one · **class: improvement**

    **Origin:** lane b11-windows, 2026-10-05, beside defect 281's repair (its final report, *Found beside*).

    **Class: improvement**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be, about an argument nobody types on purpose.

    Repaired at `35ff9024`, 2026-10-05 (lane cli12), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every verb that takes an operand said the same at the base, and `mutate` and `probe` *no file or directory is named* over nothing; each now refuses the empty word in the parser, at exit 2, as `-o ""` is.

## The repair

Repaired at `35ff9024`. `heroes check ''` said *cannot read ``*, naming an empty path as a file; at the base every verb that takes an operand, `lex` to `measure`, said the same, and `mutate` and `probe` said *no file or directory is named*. The argument parser now refuses an empty operand before anything is read, at exit 2 as defect 349 refuses `-o ""`, in the form the help gives the operand, `<file.hero>` where one is needed and `[file]` where it may be left out; after `run`'s `--` an empty word stays the program's. Its case is a compiler test in `cli/argv.hero`, asked of every command of the table that takes an operand: the empty word alone, before a file and after one.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
