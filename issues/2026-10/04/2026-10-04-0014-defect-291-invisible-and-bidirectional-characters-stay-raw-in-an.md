---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 8df5f335ac5f52ef8ac0250cbbbc9f994df0c573
github: none
---

- [x] **291 — invisible and bidirectional characters stay raw in an excerpt and a message, so U+202E reorders what the terminal shows of the line** | `x: i64 @ "<U+202E>abc"`, a type error on the line: its excerpt carries U+202E (`e2 80 ae`), which reorders what a terminal shows; U+FEFF and U+00A0 stay raw too, where 244's repair writes control characters by their code (lane b9-notext's compiler, 2026-10-04) | `selfhost/shown_char.hero` (`visible`) · `selfhost/diag_render.hero` · defect 283, whether such characters are accepted at all · **class: adjacent**

    **Origin:** lane b9-notext, 2026-10-04, reproduced on its compiler (`<scratchpad>/batch9/notext/report.md`, *Found beside* 2).

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message whose line a terminal shows in another order; the ruling 283 owes decides whether these characters reach an excerpt at all.

    Repaired at `8df5f335`, 2026-10-05, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `8df5f335`, on panel 192's Q7, landed by R13. `shown_char.visible`, the writer of every excerpt, rich and one-line message, token, syntax-tree and IR dump, wrote a control character by its code since defect 244 and every other character as itself, so U+202E on a quoted line drew it in another order, and a no-break space, a zero-width space or a byte order mark showed the reader nothing to find. It writes every character `unseen` names by its code too, `<U+202E>`, the caret padded by that width; a visible character above ASCII stays itself. Its cases are two `full/fixedbugs-291-*`, an excerpt over invisible characters on type errors and over refused bidirectional ones, each red before the repair, a surface row over the token dump of `json102/unseen.hero`, and the override's IR string in `escape251`'s row.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
