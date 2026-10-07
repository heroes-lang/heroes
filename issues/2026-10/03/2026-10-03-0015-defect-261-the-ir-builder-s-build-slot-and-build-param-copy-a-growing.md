---
kind: defect
area: ir
milestone: none
filed: 2026-10-03
commit: fcdddfa56caad12943bc56923f44d36773533b02
github: none
---

- [x] **261 — the IR builder's `build.slot` and `build.param` copy a growing table through a struct field** | 320,499 copies on `one-return-many-800` (batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*, `<scratchpad>/batch8/emit/shapes/` and `<scratchpad>/round1003d/shapes2000/`), the pattern batch 8's 230 repaired in three other places | `selfhost/ir/build.hero:254` and `:259` (`param`, `slot`) · **class: improvement**

    **Origin:** batch 8's emit lane, counted in an instrumented copy of its compiler's C (`build --emit-c`, the lane's tip `96473596`), 2026-10-03, its report's *Found beside*.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost growing with the slots' square; no program refused or wrong.

    Repaired at `fcdddfa5`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The builder's slots, parameters, blocks and path steps are lent to helpers that grow them in place and a join's predecessor list is hoisted, so `--dump-ir` is byte-identical on 20 shapes while slot copies at one-return-many-800 fall from 325,450 to 7,501 and block copies at slots-returns-800 from 2,896,203 to 18,908 (instrumented copies of both compilers).

## The repair

Repaired at `fcdddfa5`, 2026-10-07 (lane b14-ir), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The builder's slots, parameters, blocks and path steps are lent to helpers that grow them in place and a join's predecessor list is hoisted, so `--dump-ir` is byte-identical on 20 shapes while slot copies at one-return-many-800 fall from 325,450 to 7,501 and block copies at slots-returns-800 from 2,896,203 to 18,908 (instrumented copies of both compilers).

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
