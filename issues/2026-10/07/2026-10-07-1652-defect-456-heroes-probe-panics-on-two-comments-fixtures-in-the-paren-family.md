---
kind: defect
area: compiler
milestone: none
filed: 2026-10-07
commit: 428a5cfd480123649703197394c3dca9d94cc01e
github: none
---

- [x] **456 — `heroes probe` panics on two comments fixtures in the `paren` family** | `heroes probe tests/golden/surface-fixtures/comments101/ascending.hero --family paren` and the same of `closeparen.hero` each stop with *panic: array index out of range*, exit 134; every other family passes on them (lane b14-harness-b's reading on `dad2da47`'s compiler; the two runs reproduced by the coordinator before 16:52 on the trunk's compiler at `00e2eda6`, `<scratchpad>/batch14/probe456/`) | `selfhost/probe/`, the `paren` family's mutation of a line that holds a comment · **class: blocking**

    **Origin:** filed by the coordinator at 16:52 on 2026-10-07, from lane b14-harness-b's final report (*found beside*, the lane's reading *blocking*); reproduced before filing.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a crash, exit 134, in one of the compiler's own commands, which the formatter's probe by hand before a push that touches `selfhost/print/` runs (`.claude/rules/verification.md` § The formatter's probe).

    **Corrected 2026-10-07** (lane b14-text, measured on the base compiler at `dad2da47`): the panic is the parser's, and `heroes check` reaches it itself, not only `probe`'s variants: exit 134 on a program of three lines with no comment, `function f(a: (` over `        bool), b: bool)` over a body, in `parse/brace_habit.a_closer_too_many`, which read one token past the last of a body's view; the `paren` family writes that shape into `ascending.hero` and `closeparen.hero`, and every other family over the 40 comments101 fixtures exits 0.

    Repaired at `428a5cfd`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step past a run of stray closers takes the run's own closers only, a body's view laying its `dedent` and `eof` at its closing token's place, and `stray_run` asks for a token before reading it: five cases under `tests/golden/check/fixedbugs-456-*`, four red on the base at exit 134; the probe over comments101 under all seven families, 2 failures to 0.

## The repair

Repaired at `428a5cfd`, 2026-10-07 (lane b14-text), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The step past a run of stray closers takes the run's own closers only, a body's view laying its `dedent` and `eof` at its closing token's place, and `stray_run` asks for a token before reading it: five cases under `tests/golden/check/fixedbugs-456-*`, four red on the base at exit 134; the probe over comments101 under all seven families, 2 failures to 0.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
