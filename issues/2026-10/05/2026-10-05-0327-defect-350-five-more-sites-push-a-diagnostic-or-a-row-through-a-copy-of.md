---
kind: defect
area: compiler
milestone: none
filed: 2026-10-05
commit: f50b9102628108472a4ed31567c48403bc73fa2f
github: none
---

- [x] **350 — five more sites push a diagnostic or a row through a copy of the whole list, defect 146's pattern, outside the parser** | `resolve/state.push_diagnostic` (`r.out.diagnostics @ ...push`), `check/state.hero:237`, `resolve/cycles.hero` (two) and `ir/interning.hero` grow their lists by a copy each push, so N reports cost the square of N (lane b11-parse, 2026-10-05, read in the code and named in its report; unmeasured per site) | the five sites · defects 146, 267 and 343, the same pattern repaired elsewhere · **class: adjacent**

    **Origin:** lane b11-parse, 2026-10-05, beside defects 267 and 343 (its final report, *Found beside*).

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a cost that grows as the square of the reports, no message or value wrong; unmeasured per site.

    Repaired at `f50b9102`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

    Its instrument and its file's ceiling followed at `36396b71`, 2026-10-05 (lane b12-parse12): `layout/appends` reads every list of reports grown through a field place, and `selfhost/resolve/state.hero` is back under its decided 330; gated by `layout` and the net's own tests.

## The repair

Repaired at `f50b9102`, and at `36396b71`, its instrument and its file's ceiling. Five sites grew a list of reports through a field place, `r.out.diagnostics`, `out.diagnostics` twice in `resolve/cycles`, `c.out.diagnostics` and `b.out.diagnostics`, which the in-place store never reaches, so N reports cost the square of N; each now lends the list to an `append_diagnostic` whose parameter is a bare place, defect 146's rule. Measured, instructions retired: 2,000 lines of `x = 1 + true` 1.55 to 0.70 billion, 2,000 constant cycles 4.74 to 3.90, 2,000 unknown names 7.39 to 6.55; nothing said moves. `layout/appends` now reads every line that grows a list of reports through a field place, whatever holds it, its witness the base's `ir/interning.hero` put back, and `selfhost/resolve/state.hero` is back under its decided 330 lines of code. Its cases are `check/fixedbugs-350-a-resolvers-report-is-said-in-place` and `check/fixedbugs-350-a-checkers-report-is-said-in-place`; the lowering's site is a net no program reaches.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
