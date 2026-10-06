---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: 8d7b11dd25a3b73a7fe027503233fe4222c518b2
github: none
---

- [x] **286 — the rule says layer 0 holds a golden case's annotations to its `.expected`, and no hook holds them** | `.claude/rules/verification.md:544` to `:545` lists among layer 0's checks *a `tests/golden/` case's `#~` annotations are held to its `.expected`*; `grep -l -E '#~|annotation|\.expected' .claude/hooks/*` finds no hook that does (read by the coordinator at `703af779`, 2026-10-04), so a case whose marks and expectation disagree waits for the `annotations` suite | `.claude/hooks/fmt_check.py` · `.claude/rules/verification.md` § A suite is the last judge · **class: adjacent**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*, a reading of `fmt_check.py` whole); the coordinator's grep over every hook.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a rule that names a check nothing performs; the suite still judges, later than the rule says.

    Repaired at `8d7b11dd`, 2026-10-05 (the coordinator's lane b12-hook), the hook asks the `annotations` suite, narrowed to a golden case, on a write of its `.hero` or `.expected`; gated by the hook's own payloads run by hand, no instrument testing the hooks; the net is owed at the batch's close.

## The repair

Repaired at `8d7b11dd`, one change to `.claude/hooks/fmt_check.py` with defect 272, which stood in front of it. The rule said layer 0 holds a golden case's `#~` annotations to its `.expected`, and no hook did. On a write of a `tests/golden/` case's `.hero` or `.expected` the hook now asks the `annotations` suite itself, narrowed to the case, and says the suite's words at exit 2 where it fails that case; it gives no opinion where the suite selects no case, where the `.hero` or the `.expected` is not written yet, or where the run cannot be made. Its cases are the hook's own payloads, run by hand, no instrument testing the hooks: a case with one wrong mark, written as `.hero` and as `.expected`, exit 2 each with the suite's lists. The hook's cost, measured beside three lanes' work and so a size: 24.83 s cold, 6.82 and 7.02 s warm.

**Closed 2026-10-06** with batch 12 (lanes b12-str192, b12-parse12, b12-hook, b12-cli12, b12-fit12, b12-ir12 and b12-ffi13, merged into one round tree made from the trunk at `0f48f9f9`), its closing gate run on the round's head: the seed regenerated at `43e6be50` over two generations, the runtime's ABI moving from 26 to 27, 37,358,146 bytes, SHA-256 beginning `fc9751a29a1ecb8a`, its fixpoint by `cmp`, and the compiler's own tests 1,299, all passed; then the net's own tests 286, all passed, and the full net, 28 suites and `cache`, 5,862 passed and 0 failed (`probe` red in the parallel pass and 27 passed alone; `unsupported` asking its floor raised from 118 to 150, done at `51f1a18c`, then 151 passed). The census, the trunk's compiler at `0f48f9f9` against the round's over the tree's tracked files, each with its own runtime: `check --brief` over 2,655, 66 moved, every one attributed to a repair of the batch, 10 of them the trunk unable to read a `\u{...}` escape the round's source writes; `build --emit-c` over the 1,146 holding an `extern`, the ABI stamp moving every unit and 186 moved besides, every one attributed, the trunk's 27 runs at exit 2 all gone, and one wrong binding newly accepted filed as defect 392. Panel 187's R2, its instrument and plan rebuilt from the transcripts after the machine's restart that morning (the same plan on every recorded fact), the trunk's compiler against the round's over 13,594 single mutants and 16,041 pairs: one mutant worse in both arms, filed as defect 391, and one pair's second newly told. The site's build: 38 claims and 2 verb lists checked over 20 pages. The cost, instructions retired over the trunk's compiler source (its two raw SOH bytes removed in the measuring copy, which the round's compiler refuses): `check` 82,245,375,769 against the round's 58,318,643,669, and `build --emit-c` 1,032,122,020,001 against 988,241,573,073; seconds unrun, the machine not still. Linux arm64 at `43e6be50`: the compiler's own tests 1,299, all passed, and 26 suites green, `unsupported` asking the same floor and `records` and `unseen` reading no `.git` in an archive.
