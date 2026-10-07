---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: 905c2a19da3390323646640a3ea9870895764727
github: none
---

- [x] **379 — the compiler's own tests depend on the spec's paid pin being fresh** | after lane b12-str192 amended the spec (panel 192's R8), the compiler test *no operand measures the spec* and the surface row *measure runs with no operand* read red until a paid `heroes measure --refresh` re-pins the spec's digest, so a lane that amends the spec cannot keep its own tests green (its report, 2026-10-06) | the spec's pinned digest and the tests that read it (`selfhost/measure/`, `tests/harness/suite_surface.hero`) · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cleaner form: no program moves, and the refresh is owed at the round anyway.

    Repaired at `905c2a19`, the compiler-test half (lane b14-cli, lane b14-harness-b's patch applied whole), and `23db22d3`, the surface half (lane b14-harness-b), 2026-10-07, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `measure`'s own test asserts the ending the spec's pin decides and runs the real readings through the verdict with a pin it hands, and its `--refresh` refusal names `spec/reserved-words.md`, which no ceiling judges, where `CLAUDE.md` would have reached a paid call with the key set; the compiler's own tests read 1,361 all passed on a copy whose pin is stale and on this tree, whose pin is fresh, and the stale copy with the test as it stood read 1 failed, this test.

## The repair

Repaired at `905c2a19`, the compiler-test half (lane b14-cli, lane b14-harness-b's patch applied whole), and `23db22d3`, the surface half (lane b14-harness-b), 2026-10-07, gated by its cases and the compiler's own tests; the net is owed at the batch's close. `measure`'s own test asserts the ending the spec's pin decides and runs the real readings through the verdict with a pin it hands, and its `--refresh` refusal names `spec/reserved-words.md`, which no ceiling judges, where `CLAUDE.md` would have reached a paid call with the key set; the compiler's own tests read 1,361 all passed on a copy whose pin is stale and on this tree, whose pin is fresh, and the stale copy with the test as it stood read 1 failed, this test.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
