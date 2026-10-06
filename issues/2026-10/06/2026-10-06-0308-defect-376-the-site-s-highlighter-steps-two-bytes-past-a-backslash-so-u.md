---
kind: defect
area: site
milestone: none
filed: 2026-10-06
commit: 1dacca63aa12d024be3cf06134235b64ff65a672
github: none
---

- [x] **376 — the site's highlighter steps two bytes past a backslash, so `\u{1b}` in an `f` literal opens a hole at its `{`** | `site/src/lib/highlight.ts:89` advances `end += source[end] === '\\' ? 2 : 1`, so the escape panel 192's R5 adds, `\u{1b}`, is read as `\u` and then `{1b}`, which inside an `f` literal the highlighter reads as a hole: a correct program is shown wrong on the site (read by the coordinator at 03:08 on 2026-10-06; found by lane b12-str192, its site build unrun) | `site/src/lib/highlight.ts:89`, `:108` · `.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in every tool that reads the language · **class: adjacent**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program coloured wrong on a published page, a tool that re-prints the language not taught the new form.

    Repaired at `1dacca63`, 2026-10-06 (the coordinator's lane b12-hook), gated by its case run by hand red then green and the site's build; the net is owed at the batch's close.

## The repair

Repaired at `1dacca63`. The site's highlighter consumed one character after a backslash whatever followed it (`site/src/lib/highlight.ts:89` and `:108`), so in an `f` literal the escape panel 192 adds, `\u{1b}`, was read as `\u` and a hole opening at its brace, and a correct program was coloured wrong on the page. `escapeEnd` reads `\u{`, its hex digits and the closing brace, and every other escape as its one character, for plain literals and an `f` literal's pieces alike. Its case, run by hand through esbuild, red on the base and green after: `f"a\u{1b}[31m{n}"` keeps `\u{1b}[31m` in its string piece and `n` in its hole; no instrument in the tree tests the highlighter's tokens (defect 377). The site's build: exit 0, 36 claims and 2 verb lists checked.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
