---
kind: defect
area: resolve
milestone: none
filed: 2026-10-05
commit: 1883ca7d7030938bab247206c2b9fc5a308fed4a
github: none
---

- [x] **364 — a correct program's locals cost the square of their number to resolve** | `function main()` with `x0 = 1` and N lines `x<i> = x<i-1> + 1`, a correct program: `check --brief` retires 1.67 billion instructions at N = 2,000 and 11.69 billion at 8,000 (the base's compiler at `00217c39`, measured by the coordinator, 2026-10-05, `<scratchpad>/batch12/filing/` `n2-*`) | `selfhost/resolve/state.hero:86`, `lookup_local`, which scans every open scope entry, called by `declare` for the shadowing check and once per name read (a sample at N = 16,000, lane b12-parse12) · **class: adjacent**

    **Origin:** lane b12-parse12, 2026-10-05, found beside its items (its report's *found beside*); reproduced by the coordinator before 22:42 and filed under the author's instruction of that evening, meant as: *any defect found that is not an improvement goes straight into the batch*.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work on a correct program, the square of its locals, no message wrong.

    Repaired at `1883ca7d`, 2026-10-05 (lane b12-parse12), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `1883ca7d`. Every lookup of a local scanned every open scope entry (`resolve/state.lookup_local`), once per binding for the shadowing check and once per name read, so a correct chain of N locals retired 1.67 billion instructions at 2,000 and 11.69 at 8,000; and every scope's close truncated the stack by a slice that copied every entry below it, a block after each binding costing 8.03 billion at 2,000 and 52.29 at 8,000. `Scopes.at` keeps where each name's latest binding stands now, and a close lowers `Scopes.top`, the entries past it written over as bindings follow. The chain retires 1.28 billion at 2,000 and 4.73 at 8,000, the blocks 6.46 and 25.47, N parameters 2.61 at 4,000 against 6.02: linear. Its case is `check/fixedbugs-364-a-functions-locals-are-found-by-name`, the same on the base, the red being the count.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
