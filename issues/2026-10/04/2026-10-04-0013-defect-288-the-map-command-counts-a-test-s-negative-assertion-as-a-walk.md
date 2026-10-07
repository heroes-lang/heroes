---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: e21de4e65868d1daaa219b90bd7944998c3a0a23
github: none
---

- [x] **288 — the map command counts a test's negative assertion as a walk, so the published rows name suites that walk nothing there** | `.claude/rules/verification.md`'s map command greps each suite file for a path's literal, so a suite whose test asserts it does NOT walk a directory is listed as walking it: the rows for `tests/golden/check/**` and `tests/golden/unsupported/**` name `canonical`, which walks neither (lane b9-harness, 2026-10-04, a reading) | `.claude/rules/verification.md` § The map is not written here · **class: improvement**

    **Origin:** lane b9-harness, 2026-10-04 (its reply's *found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a map over-listing, which runs a suite too many and never one too few; no program moves.

    Repaired at `e21de4e6`, 2026-10-07 (lane b14-hooks), gated by the command itself, run as documented, and `records`; the net is owed at the batch's close. The map command reads a suite's code with its `test` blocks and its comments left out; against its old self it moved three rows of output, `canonical` losing the four golden directories its test asserts it does not walk, `unseen` the seed it exempts and `records` three literals of its tests, and the table's rows for `tests/golden/check/**`, `unsupported/**`, `permissive/**` and `full/**` no longer name `canonical`.

## The repair

Repaired at `e21de4e6`, 2026-10-07 (lane b14-hooks), gated by the command itself, run as documented, and `records`; the net is owed at the batch's close. The map command reads a suite's code with its `test` blocks and its comments left out; against its old self it moved three rows of output, `canonical` losing the four golden directories its test asserts it does not walk, `unseen` the seed it exempts and `records` three literals of its tests, and the table's rows for `tests/golden/check/**`, `unsupported/**`, `permissive/**` and `full/**` no longer name `canonical`.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
