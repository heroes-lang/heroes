---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: 2a2047748c95a9103f03e76736bb51d97afe6c1b
github: none
---

- [x] **258 — `cursor.is_invisible` costs the square of the block depth** | 4,014,367 calls on `match-nested-1000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/cursor.hero:194` (`is_invisible`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square; no program refused or wrong.

    Repaired at `2a204774` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The last walk back is kept on the cursor (`stream_tables`) and a walk that meets where it began goes on from where it ended, the parser's writes to its stream going through `stream_tables.relaid`: `is_invisible` asked 3,362, 6,362 and 12,362 times on `match` nested 250, 500 and 1,000 deep, where it was asked 253,896, 1,007,396 and 4,014,396 (reproduced on `dad2da47`), and `parse selfhost/main.hero` 0.16% dearer.

## The repair

Repaired at `2a204774` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The last walk back is kept on the cursor (`stream_tables`) and a walk that meets where it began goes on from where it ended, the parser's writes to its stream going through `stream_tables.relaid`: `is_invisible` asked 3,362, 6,362 and 12,362 times on `match` nested 250, 500 and 1,000 deep, where it was asked 253,896, 1,007,396 and 4,014,396 (reproduced on `dad2da47`), and `parse selfhost/main.hero` 0.16% dearer.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
