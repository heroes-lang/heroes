---
kind: defect
area: parse
milestone: none
filed: 2026-10-03
commit: 5ddf3f4d320628bc6f0992c73bc08690eb02d15f
github: none
---

- [x] **257 — the parser's `pairing.is_closer` costs the square of the bracket depth** | 40,040,825 calls on `record-literal-2000` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`) | `selfhost/parse/pairing.hero:54` (`is_closer`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the nesting's square; no program refused or wrong.

    Repaired at `5ddf3f4d` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On `dad2da47` the item's own shape is linear since defect 344 (`b93bf7f8`): a record literal 2,000 deep asks `token.is_closer` 8,984 times. The square was left where no walk paired the cursor, a view of braces and a plain literal's hole, and each is paired as it is made now: in braces 2,000 deep 16,016,994 calls to 16,998 and `check --brief` 17.39 billion instructions retired to 0.98; in a hole 1,000 deep 1,003,973 calls to 1,974.

## The repair

Repaired at `5ddf3f4d` (2026-10-07, lane b14-parse), gated by its cases and the compiler's own tests; the net is owed at the batch's close. On `dad2da47` the item's own shape is linear since defect 344 (`b93bf7f8`): a record literal 2,000 deep asks `token.is_closer` 8,984 times. The square was left where no walk paired the cursor, a view of braces and a plain literal's hole, and each is paired as it is made now: in braces 2,000 deep 16,016,994 calls to 16,998 and `check --brief` 17.39 billion instructions retired to 0.98; in a hole 1,000 deep 1,003,973 calls to 1,974.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
