---
kind: defect
area: parse
milestone: none
filed: 2026-10-04
commit: 3c8ca6668eea5fa733bf5ba109955804818958eb
github: none
---

- [x] **313 — an outer list never closed, its inner list closed elsewhere, may give a spurious message, unrun** | defect 204's shape nested: an outer `[` never closed around an inner list the lexer closes elsewhere; the lane suspected a spurious message and did not run it (2026-10-04): a question until the shape is run | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defect 204 · **class: improvement**

    **Origin:** lane b9-recovery, 2026-10-04, reproduced on its compiler (its final reply's *Found beside*; scratch `<scratchpad>/batch9/recovery/`), an unrun question.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed; if run and spurious, `adjacent`.

    Repaired at `3c8ca666` (2026-10-07, lane b14-parse), gated by its cases; the net is owed at the batch's close. Measured on `dad2da47`, six shapes, `check` and `check --permissive` alike: no spurious message. The outer `[` keeps the lexer's own report, still open where a line ends its reach, since an opener the inner list stands in is named with it only where a closer further down closes that opener too (`bracket_head.openers_around`, defect 307's rule); the binding's report names the inner `[` and where its `]` goes, and the stray closer is told. The case pins the 18 reports.

## The repair

Repaired at `3c8ca666` (2026-10-07, lane b14-parse), gated by its cases; the net is owed at the batch's close. Measured on `dad2da47`, six shapes, `check` and `check --permissive` alike: no spurious message. The outer `[` keeps the lexer's own report, still open where a line ends its reach, since an opener the inner list stands in is named with it only where a closer further down closes that opener too (`bracket_head.openers_around`, defect 307's rule); the binding's report names the inner `[` and where its `]` goes, and the stray closer is told. The case pins the 18 reports.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
