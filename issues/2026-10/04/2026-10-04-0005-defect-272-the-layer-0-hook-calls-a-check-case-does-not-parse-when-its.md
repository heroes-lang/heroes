---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: 8d7b11dd25a3b73a7fe027503233fe4222c518b2
github: none
---

- [x] **272 — the layer-0 hook calls a check case *does not parse* when its intended diagnostic makes `fmt` refuse it** | writing `tests/golden/check/fixedbugs-225-a-climb-to-the-root-is-a-known-cost.hero`, whose `machine_locked_path` is annotated: the hook answered *does not parse; the compiler says: ... error: refusing to format a file with diagnostics*, exit 2, while `check`, `annotations` and `fixes` read the case 1 and 0 each (2026-10-04); every case holding a parse-stage refusal, `ffi-a-group-head-names-not-locates` on the trunk's compiler among them, draws the same | `.claude/hooks/fmt_check.py:86` to `:94` · **class: improvement**

    **Origin:** the coordinator, 2026-10-04, writing defect 225's pin.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument's false alarm on every such case, which teaches its reader to pass it by; no program moves.

    Repaired at `8d7b11dd`, 2026-10-05 (the coordinator's lane b12-hook), a golden case whose `#~` marks claim diagnostics is judged on its marks where `fmt` refuses it, an unmarked one still told it does not parse; gated by the hook's own payloads run by hand, no instrument testing the hooks; the net is owed at the batch's close.

## The repair

Repaired at `8d7b11dd`, one change to `.claude/hooks/fmt_check.py` with defect 286. The layer-0 hook answered *does not parse* for every golden case holding a parse-stage refusal, since `fmt` refuses to format a file with diagnostics. A golden case whose `#~` marks claim diagnostics is judged on its marks now where `fmt` refuses it, and a golden case with no marks that does not parse is still told so. Its cases are the hook's own payloads, run by hand, no instrument testing the hooks: `fixedbugs-130`'s `.hero` and `.expected`, exit 0 each where the base said *does not parse*, and an unmarked case that does not parse, exit 2 *does not parse*.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
